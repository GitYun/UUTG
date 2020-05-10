/**
 * \file uutg.c
 * \author vEmagic (admin@vemagic.com)
 * \brief Unity Uint Test Generator
 * \version 0.0.1
 * \date 2020-04-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include "mpc.h"
#include "utils.h"
#include "strplus.h"
#include "csfp.h"
#include "uutg.h"

static char* readFile(FILE* fp);
static void UnitTestTemplateGenerator(char *outputPath, char *filename, char **ppsFuncs, int count);
static char* dirPathEndWithSlash(char *dirPath);
static void UnitRunnerTemplateGenerator(tTestGroup *group, int count, const char *outputPath);
static void UnitMainTemplateGenerator(tTestGroup *group, int count, const char *outputPath);
static void PrintGroupMessage(tTestGroup *group, int count);
static void GroupMemoryFree(tTestGroup *group, int count);

void UnitTestGenerator(char *inputPath, char *outputPath, char *suffix)
{
    if (!SL_StringIsDirPath(inputPath))
        return;

    if (!SL_StringIsDirPath(outputPath) && mkdir(outputPath))
        return;
    
    inputPath = dirPathEndWithSlash(inputPath);
    outputPath = dirPathEndWithSlash(outputPath);
    SL_StringReplace(inputPath, "\\", "/");
    SL_StringReplace(outputPath, "\\", "/");
    
    char **ppsFiles = NULL;
    int filesCount = 0;
    char **ppsFunctions = NULL;
    int funcCount = 0;
    tTestGroup *group = calloc(1, sizeof(tTestGroup));
    int groupCount = 0;

    GetDirentInfo(inputPath, &ppsFiles, &filesCount, suffix);
    SL_StringArrayPrint(stdout, ppsFiles, filesCount);

    for (int i = 0; i < filesCount; ++i)
    {
        char *file = ppsFiles[i];
        char *fileNameWithoutSuffix = NULL;
        SL_StringReplace(file, "\\", "/");
        fileNameWithoutSuffix = SL_StringTrimString(strdup(strrchr(file, '/') + 1), suffix);

        ppsFunctions = (char **)calloc(1, sizeof(char *));
        GetFuncsDeclare(file, &ppsFunctions, &funcCount);
        fprintf(stdout, "\nFile :%s", file);
        SL_StringArrayPrint(stdout, ppsFunctions, funcCount);
        UnitTestTemplateGenerator(outputPath, fileNameWithoutSuffix, ppsFunctions, funcCount);

        if (funcCount > 0)
        {
            ++groupCount;
            group = realloc(group, groupCount * sizeof(tTestGroup));
            memset(&group[groupCount - 1], 0, sizeof(tTestGroup));
            group[groupCount - 1].group = SL_StringAppend(fileNameWithoutSuffix, "_Test", NULL);
            
            for (int it = 0; it < funcCount; ++it)
                SL_StringArrayLog(ppsFunctions[it], &group[groupCount - 1].name, &group[groupCount - 1].nameCount);
        }

        free(fileNameWithoutSuffix);
        SL_StringArrayFree(&ppsFunctions, &funcCount);
    }
    SL_StringArrayFree(&ppsFiles, &filesCount);

    char *outputRunner = SL_StringAppend(outputPath, "UnityTestRunner.c", NULL);
    char *outputMain = SL_StringAppend(outputPath, "main_temp.c", NULL);
    UnitRunnerTemplateGenerator(group, groupCount, outputRunner);
    UnitMainTemplateGenerator(group, groupCount, outputMain);

    GroupMemoryFree(group, groupCount);
    free(outputRunner);
    free(outputMain);
    free(inputPath);
    free(outputPath);
}

void GetFuncsDeclare(const char *filename, char ***ppsFuncs, int *count)
{
    mpc_parser_t * Modifier = mpc_new("modifier");
    mpc_parser_t * CombType = mpc_new("combType");
    mpc_parser_t * DataType = mpc_new("dataType");
    mpc_parser_t * Ident    = mpc_new("ident");
    mpc_parser_t * Var      = mpc_new("var");
    mpc_parser_t * Args     = mpc_new("args");
    mpc_parser_t * Declare  = mpc_new("declare");

    mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
        "modifier : \"extern\" | \"static\" <ident>? | \"const\" ;            \n"
        "combType : \"struct\" | \"union\" | \"enum\" ;                       \n"
        "dataType : <modifier>? <combType>? ((<ident> (<modifier>? '*' <modifier>?)*) | \"...\") ;   \n"
        "ident    : /[a-zA-Z_][a-zA-Z0-9_]*/ ;                                \n"
        "var      : <dataType> ( ('(' '*' <ident>? ')' '(' <args> ')') | <ident>)? ; \n"
        "args     : <var>? (',' <var>?)* ;                                    \n"
        "declare  : (<modifier>? <dataType> <ident> '(' <args> ')' ';'?)* ;   \n",
        Modifier, CombType, DataType, Ident, Var, Args, Declare, NULL);

    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(filename, "rb");
    for (;;)
    {
        char *input = readFile(fp);
        if (feof(fp) || input == NULL)
            break;

        if (strcmp(input, "") != 0) // The input string not equal none string
        {
            mpc_result_t r;
            if (mpc_parse(filename, input, Declare, &r))
            {
                mpc_ast_t *ast = r.output;
                if (ast != NULL && ast->children_num > 0)
                {
                    // mpc_ast_print(ast);
                    for (int i = 0; i < ast->children_num; ++i)
                    {
                        if (strstr(ast->children[i]->tag, "dataType"))
                        {
                            SL_StringArrayLog(ast->children[i+1]->contents, ppsFuncs, count);
                            // printf("%s\n", ast->children[i+1]->contents);
                            break;
                        }
                    }
                }
                mpc_ast_delete(r.output);
            }
            else
            {
                // mpc_err_print(r.output);
                mpc_err_delete(r.output);
            }
        }
        free(input);
    }

    mpc_cleanup(7, Modifier, CombType, DataType, Ident, Var, Args, Declare);
    fclose(fp);
}

static char* readFile(FILE* fp)
{
#define LINE_STRING_LENGTH_MAX  2048
    int inputMaxLength = 0;
    char * input = calloc(1, sizeof(char));
    int length = 0;

    for (;;)
    {
        if (length + 1 >= inputMaxLength)
        {
            inputMaxLength += (LINE_STRING_LENGTH_MAX >> 1);
            input = realloc(input, sizeof(char) * inputMaxLength);
            if (input == NULL)
                return NULL;
        }

        char * buf = fgets(input + length, inputMaxLength - length, fp);
        if (feof(fp)) // End of file
        {
            free(input);
            input = NULL;
            break;
        }

        if (buf == NULL) // Return null of read file
            break;

        input = SL_StringTrimLeadWhitespace(input);
        length = strlen(input);
        if (buf[0] == '#' || !strcmp(buf, "\r\n") || !strcmp(buf, "\n") || strstr(buf, "//") || strstr(buf, "*/"))
        {
            memset(buf, 0, strlen(buf));
            length = 0;
            if (buf == input)
                continue;
            else
                break;
        }
        
        if (strpbrk(input, "{};"))
        {
            // Remove the CR, LF and left bracket symbol
            char * pos = strchr(input, '{');
            char * substr = strtok(input, "{");
            char * ret = calloc(1, sizeof(char));
            do {
                ret = realloc(ret, sizeof(char) * (strlen(ret) + strlen(substr) + 1));
                strncat(ret, substr, strlen(substr) + 1);
            } while(!pos && (substr = strtok(NULL, "{")));

            free(input);
            input = ret;
            break;
        }
    }

    return input;
}

static char* dirPathEndWithSlash(char *dirPath)
{
    char *ret = NULL;
    if (SL_StringWithEnds(dirPath, "\\/") < 0)
        ret = SL_StringAppend(dirPath, "/", NULL);
    else
        ret = strdup(dirPath);
    
    return ret;
}

// 生成单元测试用例文件
static void UnitTestTemplateGenerator(char *outputPath, char *filename, char **ppsFuncs, int count)
{
    if (outputPath == NULL || filename == NULL || ppsFuncs == NULL || count < 1)
        return;

    char *file = SL_StringAppend(outputPath, filename, "_Test.c", NULL);
    FILE *fp = fopen(file, "w+");

    if (fp)
    {
        char *header = SL_StringAppend("#include \"", filename, ".h\"", NULL);
        char *headers[2] = {"#include \"unity_fixture.h\"", header};
        char *testGroupName = SL_StringAppend(filename, "_Test", NULL);

        outputCFileGenerateMessage(fp, " * THE FILE IS GENERATED by TGen.\n");
        outputCFileHead(fp, headers, 2);
        outputCFileBlankLine(fp);

        outputCFileFunctionCall(fp, "TEST_GROUP(", &testGroupName, 1);
        outputCFileBlankLine(fp);

        char *testConfig[2] = {"TEST_SETUP(", "TEST_TEAR_DOWN("};
        for (int i = 0; i < 2; ++i)
        {
            outputCFileFunctionStart(fp, testConfig[i], &testGroupName, 1);
            outputCFileBlankLine(fp);
            outputCFileFunctionEnd(fp, "}");
            outputCFileBlankLine(fp);
        }
        
        for (int idx = 0; idx < count; ++idx)
        {
            char *args[2] = {testGroupName, ppsFuncs[idx]};
            outputCFileFunctionStart(fp, "TEST(", args, 2);
            outputCFileBlankLine(fp);
            outputCFileFunctionEnd(fp, "}");
            outputCFileBlankLine(fp);
        }
        fclose(fp);
        free(header);
        free(testGroupName);
    }

    free(file);
}

// 生成测试运行容器源文件
static void UnitRunnerTemplateGenerator(tTestGroup *group, int count, const char *outputPath)
{
    FILE *fp = fopen(outputPath, "w+");
    if (fp)
    {
        char *head = "#include \"unity_fixture.h\"";
        char *testRunner = "TEST_GROUP_RUNNER(";
        char *testCase = "\tRUN_TEST_CASE(";

        outputCFileGenerateMessage(fp, " * THE FILE IS GENERATED by TGen, DO NOT EDITE IT.\n");
        outputCFileHead(fp, &head, 1);
        outputCFileBlankLine(fp);
        for (int idx = 0; idx < count; ++idx)
        {
            char *groupName = group[idx].group;
            outputCFileFunctionStart(fp, testRunner, &groupName, 1);

            for (int cnt = 0; cnt < group[idx].nameCount; ++cnt)
            {
                char *testName = group[idx].name[cnt];
                char *args[2] = {groupName, testName};
                outputCFileFunctionCall(fp, testCase, args, 2);
            }

            outputCFileFunctionEnd(fp, "}");
            outputCFileBlankLine(fp);
        }
        fclose(fp);
    }
}

// 生成测试main文件
static void UnitMainTemplateGenerator(tTestGroup *group, int count, const char *outputPath)
{
    FILE *fp = fopen(outputPath, "w+");
    if (fp)
    {
        char *head = "#include \"unity_fixture.h\"";
        char *RunTestGroup = "\tRUN_TEST_GROUP(";
        char *RunTestAll = "static void RunTestAll(void)";
        char *main = "int main(int argc, char *argv[])";
        char *unitymain = "\treturn UnityMain(argc, argv, RunTestAll);\n";
        size_t size = sizeof(char);

        outputCFileHead(fp, &head, 1);
        outputCFileBlankLine(fp);
        outputCFileFunctionStart(fp, RunTestAll, NULL, 0);
        for (int idx = 0; idx < count; ++idx)
        {
            char *groupName = group[idx].group;
            outputCFileFunctionCall(fp, RunTestGroup, &groupName, 1);
        }
        outputCFileFunctionEnd(fp, "}");

        outputCFileBlankLine(fp);

        outputCFileFunctionStart(fp, main, NULL, 0);
        fwrite(unitymain, size, strlen(unitymain), fp);
        outputCFileFunctionEnd(fp, "}");

        fclose(fp);
    }
}

// 打印测试组信息到标准输出
static void PrintGroupMessage(tTestGroup *group, int count)
{
    fprintf(stdout, "\nGroup count: %d", count);
    for (int idx = 0; idx < count; ++idx)
    {
        fprintf(stdout, "\n%s: %d Functions\n", group[idx].group, group[idx].nameCount);
        SL_StringArrayPrint(stdout, group[idx].name, group[idx].nameCount);
    }
}

static void GroupMemoryFree(tTestGroup *group, int count)
{
    for (int idx = 0; idx < count; ++idx)
    {
        SL_StringArrayFree(&group[idx].name, &group[idx].nameCount);
        free(group[idx].group);
    }
    free(group);
}

#if 0
mpc_parser_t** MPC_ParserCreate(const char *parserRule)
{
    if (parserRule == NULL)
        return NULL;

    int count = 0;
    mpc_parser_t **array = (mpc_parser_t **)calloc(1, sizeof(mpc_parser_t *));
    char *rule = strdup(parserRule);
    if (array == NULL)
    {
        free(array);
        free(rule);
        return NULL;
    };

    // SL_StringReplace(rule, " \n", "");
    SL_StringTrim(rule, " \n");
    char **stringSet = SL_StringDelimSubstringReverse(rule, ":", ";");
    free(rule);

    char **ruleName = stringSet;
    while (ruleName != NULL)
    {
        array = (mpc_parser_t **)realloc(array, ++count * sizeof(mpc_parser_t *));
        array[count - 1] = mpc_new(*ruleName);
        ++ruleName;
    }

    return array;
}
#endif