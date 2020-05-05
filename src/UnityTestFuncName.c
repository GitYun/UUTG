/**
 * \file UnityTestFuncName.c
 * \author vEmagic (admin@vemagic.com)
 * \brief 
 * \version 0.0.1
 * \date 2020-05-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include "mpc.h"
#include "stringHelper.h"
#include "UnitTestGenerator.h"
#include "UnityTestFuncName.h"

static void GetGroupNameAndTestName(mpc_ast_t *ast, struct _test_group_ **ppsTestGroup, int *groupCount);

int GetTestFuncDeclare(char **ppsFiles, int fileCount, char ***pppsFuncs, int *count, tTestGroup **ppsTestGroup)
{
    int testGroupCount = 0;
    if (ppsFiles == NULL || fileCount < 1)
        return 0;
    
    mpc_parser_t * testMacro = mpc_new("testmacro");
    mpc_parser_t * ident     = mpc_new("ident");
    mpc_parser_t * declare   = mpc_new("declare");

    mpc_err_t * err = mpca_lang(MPCA_LANG_DEFAULT,
        "testmacro : \"TEST\" | \"IGNORE_TEST\" ;              \n"
        "ident     : /[a-zA-Z_][a-zA-Z0-9_]*/ ;                \n"
        "declare   : (<testmacro> '(' <ident> ',' <ident> ')')* ; \n",
        testMacro, ident, declare, NULL);
    
    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
        exit(EXIT_FAILURE);
    }

    for (int it = 0; it < fileCount; ++it)
    {
        char *file = ppsFiles[it];
        if (!file || !SL_StringIsFilePath(file))
            continue;

        FILE *fp = fopen(file, "rb");
        char *input = readFile(fp);
        
        while (!feof(fp) && input)
        {
            if (strcmp(input, "") != 0)
            {
                mpc_result_t r;
                if (mpc_parse("TestFunc", input, declare, &r))
                {
                    mpc_ast_t *ast = r.output;
                    if (ast != NULL && ast->children_num == 6)
                    {
                        // mpc_ast_print(ast);
                        char *funcDeclare = SL_StringAppend(NULL,
                                            ast->children[0]->contents, 
                                            ast->children[1]->contents,
                                            ast->children[2]->contents,
                                            ast->children[3]->contents,
                                            ast->children[4]->contents,
                                            ast->children[5]->contents, NULL);
                        SL_StringArrayLog(funcDeclare, pppsFuncs, count);
                        free(funcDeclare);
                        GetGroupNameAndTestName(ast, ppsTestGroup, &testGroupCount);
                    }
                    mpc_ast_delete(r.output);
                }
                else
                {
                    mpc_err_delete(r.error);
                }
            }
            free(input);
            input = readFile(fp);
        }

        fclose(fp);
    }

    mpc_cleanup(3, testMacro, ident, declare);
    return testGroupCount;
}

int GetGroupAndTestName(char **ppsFuncs, int count, struct _test_group_ **ppsTestGroup)
{
    struct _test_group_ * group = NULL;
    int groupCount = 0;

    for (int idx = 0; idx < count; ++idx)
    {
        int len = 0;
        char *leftBracket = strchr(ppsFuncs[idx], '(');
        char *comma = strchr(ppsFuncs[idx], ',');
        char *rightBracket = strrchr(ppsFuncs[idx], ')');

        if (leftBracket && comma && rightBracket)
        {
            char *has = NULL;
            for (int i = 0; i < groupCount; ++i)
            {
                has = strstr(leftBracket + 1, group[i].group);
            }

            struct _test_group_ *testGroup = NULL;
            if (!has && ++groupCount &&
                (NULL != (group = (struct _test_group_ *)realloc(group, groupCount * sizeof(struct _test_group_)))))
            {
                testGroup = &group[groupCount - 1];
                memset(testGroup, 0, sizeof(struct _test_group_));
                len = comma - leftBracket;
                if (NULL != (testGroup->group = (char *)calloc(len, sizeof(char))))
                {
                    strncpy(testGroup->group, leftBracket + 1, len);
                    testGroup->group[len - 1] = '\0';
                }
            }

            if (group != NULL && groupCount > 0)
            {
                testGroup = &group[groupCount - 1];
                ++testGroup->nameCount;
                testGroup->name = (char **)realloc(testGroup->name, testGroup->nameCount * sizeof(char *));
                if (testGroup->name != NULL)
                {
                    char **testName = &testGroup->name[testGroup->nameCount - 1];
                    len = rightBracket - comma;
                    if (NULL != (*testName = (char *)calloc(len, sizeof(char))))
                    {
                        strncpy(*testName, comma + 1, len);
                        (*testName)[len - 1] = '\0';
                    }
                }
            }
        }
    }

    *ppsTestGroup = group;
    return groupCount;    
}

static void GetGroupNameAndTestName(mpc_ast_t *ast, struct _test_group_ **ppsTestGroup, int *groupCount)
{
    struct _test_group_ *group = *ppsTestGroup;

    bool bGroupExisted = false;
    int groupIdx = 0;

    for (; groupIdx < *groupCount; ++groupIdx)
    {
        bGroupExisted = !strcmp(ast->children[2]->contents, group[groupIdx].group);
        if (bGroupExisted)
            break;
    }

    if (!bGroupExisted && 
        (NULL != (group = (tTestGroup *)realloc(group, ++*groupCount * sizeof(tTestGroup)))))
    {
        tTestGroup *tmp = &group[*groupCount - 1];
        memset(tmp, 0, sizeof(tTestGroup));
        tmp->group = strdup(ast->children[2]->contents);
        tmp->name = (char **)calloc(1, sizeof(char *));
        groupIdx = *groupCount - 1;
    }

    if (group && *groupCount > 0 && *groupCount > groupIdx)
    {
        tTestGroup *tmp = &group[groupIdx];
        ++tmp->nameCount;
        tmp->name = (char **)realloc(tmp->name, tmp->nameCount * sizeof(char *));
        if (tmp->name)
        {
            tmp->name[tmp->nameCount - 1] = strdup(ast->children[4]->contents);
        }
    }

    *ppsTestGroup = group;
}