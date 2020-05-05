/**
 * \file UnitTestGenerator.c
 * \author vEmagic (admin@vemagic.com)
 * \brief 
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
#include "stringHelper.h"
#include "CCodeParse.h"
#include "GeneratorOutputPrivate.h"
#include "UnitTestGenerator.h"
#include "UnityTestFuncName.h"
#include "UnitTestRunnerGenerator.h"

static void UnitTestTemplateGenerator(char *outputPath, char *filename, char **ppsFuncs, int count);

void UnitTestGenerator(char *inputPath, char *outputPath, char *suffix)
{
    if (!SL_StringIsDirPath(inputPath))
        return;

    if (!SL_StringIsDirPath(outputPath) && mkdir(outputPath))
        return;
    
    char **ppsFiles = NULL;
    int filesCount = 0;
    CCP_SetCwd(inputPath);
    CCP_GetDirFileInfo(inputPath, &ppsFiles, &filesCount, NULL, suffix);
    outputPath = strdup(outputPath);
    SL_StringReplace(outputPath, "\\", "/");

    char **ppsFunctions = NULL;
    int funcCount = 0;
    for (int i = 0; i < filesCount; ++i)
    {
        char *file = ppsFiles[i];
        ppsFunctions = (char **)calloc(1, sizeof(char *));

        SL_StringReplace(file, "\\", "/");
        GetFuncsDeclare(file, &ppsFunctions, &funcCount);
        SL_StringTrimString(file, suffix);
        UnitTestTemplateGenerator(outputPath, strrchr(file, '/'), ppsFunctions, funcCount);
        SL_StringArrayFree(&ppsFunctions, &funcCount);
    }
    CCP_Free(&ppsFiles, &filesCount);
    
    CCP_SetCwd(outputPath);
    CCP_GetDirFileInfo(NULL, &ppsFiles, &filesCount, "_Test", ".c");

    tTestGroup *group = calloc(1, sizeof(tTestGroup));
    int groupCount = GetTestFuncDeclare(ppsFiles, filesCount, &ppsFunctions, &funcCount, &group);
    CCP_PrintfMessage(ppsFiles, filesCount);
    CCP_PrintfMessage(ppsFunctions, funcCount);

    RunnerGenerator_GeneratorRunnerFile(&group, groupCount, "UnityTestRunner.c");
    RunnerGenerator_GeneratorMainFile(&group, groupCount, outputPath, "main_temp.c");
    PrintGroupMessage(&group, groupCount);

    RunnerGenerator_TestGroupAndTestnameDestory(&group, groupCount);
    CCP_Free(&ppsFunctions, &funcCount);
    CCP_Free(&ppsFiles, &filesCount);
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

char* readFile(FILE* fp)
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

static void UnitTestTemplateGenerator(char *outputPath, char *filename, char **ppsFuncs, int count)
{
    if (outputPath == NULL || filename == NULL || ppsFuncs == NULL || count < 1)
        return;

    char *path = (char *)outputPath;
    char *file = NULL;

    SL_StringTrim(filename, "\\/");

    if (SL_StringWithEnds(path, "\\/") < 0)
        path = SL_StringAppend(path, "/", NULL);
    if (path != NULL)
    {
        file = SL_StringAppend(path, filename, "_Test.c", NULL);
        if (path != outputPath)
            free(path);
        path = NULL;
    }

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
#endif