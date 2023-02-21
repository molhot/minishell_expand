/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: satushi <satushi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 22:50:28 by user              #+#    #+#             */
/*   Updated: 2023/02/18 05:34:20 by satushi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*************************************
 * 
 * 
 * 
 *  		TEST PARSER
 * 
 * 
 * 
 * ***********************************/

#include <stdio.h>
#include <assert.h>

t_node  *new_node(t_node_kind kind);
t_token      *tokdup(t_token *tok);
void    free_node(t_node *node);

void    test_append_tok(void)
{
        char *line = "echo hoge";
        t_token *t1 = tokenizer(line);
        t_token *t2 = new_token("huga", TK_WORD);
        append_tok(&t1, t2);
        //t_token *itr;
        //itr = t1;

        assert(strcmp(t1->word, line) == 0);
        assert(strcmp(t1->next->next->word, "huga") == 0);
        assert(t1->next->kind == TK_EOF);
        assert(t1->next->next->kind == TK_WORD);

        t_node *n = new_node(ND_SIMPLE_CMD);
        append_tok(&n->command->args, tokdup(t1));
        printf("%s\n", n->command->args->word);
}

void    test_parse(void)
{
        char    *line = "echo";
        t_token *t = tokenizer(line);
        printf("%s\n", t->word);
        t_node  *n = parse(t);
        // assert(strcmp(n->command->args->word, "echo hoge"));
        //assert(strcmp(n->command->args->next->word, "hoge"));
        free_node(n);
        free_token(t);

        line = "< hoge";
        t = tokenizer(line);
        n = parse(t);
        assert(strcmp((*n->command->redirect)->file_path, "hoge") == 0);
        free((*n->command->redirect)->file_path);
        free((*n->command->redirect));
        free(n->command->redirect);
        free_token(t);
      
//
//      parse_redirect(&n->command->redirect, &t);
//      assert(strcmp(n->command->redirect->file_path, "hoge") == 0);
//      assert(n->command->redirect->type == IN);
//
//      line = "cat < hoge";
//      t = tokenizer(line);
//      n = parse(t);
//      assert(strcmp(n->command->args->word, "cat") == 0);
//      assert(n->command->redirect->type == IN);
//      assert(strcmp(n->command->redirect->file_path, "hoge") == 0);

        // Pipeのパース未実装
        //line = "echo hoge | echo huga";
        //t = tokenizer(line);
        //n = parse(t);
}
int main(void)
{
        test_parse();
        //test_append_tok();
}
