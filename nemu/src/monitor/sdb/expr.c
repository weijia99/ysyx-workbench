/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

//this is pa1.2,you should add regex&token
//using Inverse Poland to calculate the answer
enum {
  TK_NOTYPE = 256, TK_EQ,TK_REG,TK_NUMS,AND,OR,NE,HEX,POINT

  /* TODO: Add more token types */
//32长度的reg,在riscv里面定义的寄存器
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {
//    这里是写具体的正则样式
  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces,这个就是正则,表示空格+
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"!=",NE},
  {"\\*",'*'},
  {"/",'/'},
  {"-",'-'},
  {"!", '!'},
  {"\\(", '('},
  {"\\)", ')'},
  {"&&",AND},
  {"\\|\\|",OR},
  {"[0-9]+",TK_NUMS},
  {"0x[0-9a-fA-F]+",HEX},
  {"\\$0",TK_REG},
  {"\\$ra",TK_REG},
  {"\\$[sgt]p",TK_REG},
  {"\\$t[0-6]",TK_REG},
  {"\\$[as][0-7]",TK_REG},
  {"\\$[s][1][0-1]",TK_REG},
  {"\\$[s][8-9]",TK_REG},
  {"\\*\\(.*\\)",POINT}

};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        //加入到tokens数组
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        
        //空格跳过,其余的加入
        switch (rules[i].token_type) {
            case TK_NOTYPE:
                break;
          default:
              for(int i=0;i<substr_len;i++){
                  tokens[nr_token].str[i]=substr_start[i];
              }
              tokens[nr_token].type=rules[i].token_type;
              nr_token++;
                break;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  TODO();

  return 0;
}
