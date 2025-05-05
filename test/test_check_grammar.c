#include "lexer.h"
#include "t_minishell.h"
#include "unity.h"
#include <stdlib.h>

// using source files for testing
TEST_SOURCE_FILE("ft_calloc.c")
TEST_SOURCE_FILE("ft_lstnew.c")
TEST_SOURCE_FILE("ft_lstadd_back.c")
TEST_SOURCE_FILE("ft_strlen.c")
TEST_SOURCE_FILE("ft_substr.c")
TEST_SOURCE_FILE("ft_lstclear.c")
TEST_SOURCE_FILE("ft_bzero.c")
TEST_SOURCE_FILE("ft_lstdelone.c")
TEST_SOURCE_FILE("ft_lstlast.c")
TEST_SOURCE_FILE("ft_strncmp.c")
TEST_SOURCE_FILE("ft_itoa.c")
TEST_SOURCE_FILE("dictionary.c")
TEST_SOURCE_FILE("lexicize_func_sign.c")
TEST_SOURCE_FILE("lexicize_text.c")
TEST_SOURCE_FILE("lexing__utils.c")
TEST_SOURCE_FILE("lexer.c")
TEST_SOURCE_FILE("grammar__prefix.c")
TEST_SOURCE_FILE("grammar__utils.c")
TEST_SOURCE_FILE("grammar__next_token.c")
TEST_SOURCE_FILE("grammar_binop_pipe.c")
TEST_SOURCE_FILE("grammar_quote.c")
TEST_SOURCE_FILE("grammar_redirect.c")
TEST_SOURCE_FILE("grammar_subshell.c")
TEST_SOURCE_FILE("grammar_subshell_to_redirect.c")
TEST_SOURCE_FILE("grammar_text.c")
TEST_SOURCE_FILE("check_grammar.c")

void	setUp(void)
{
	// テスト前の準備があればここで実行
}

void	tearDown(void)
{
	// テスト後のクリーンアップがあればここで実行
}

static char	*replase_first_ifs_with_null(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_ifs(str[i]))
		{
			str[i] = '\0';
			break ;
		}
		i++;
	}
	return (str);
}

// エラー時のトークンポインタを検証するヘルパー関数
static void	test_grammar(int expected_result, const char *input,
		int expected_id, t_token_type expected_type, const char *expected_value,
		int expected_heredoc_count, int line_num)
{
	t_list	*token_list;
	int		subshell_count;
	int		result;
	t_token	*error_token;
	char	*err_text;
	int		heredoc_count;
	int		heredoc_flag;

	// printf("line_num: %d\n", line_num);
	// char	*line_num_str;
	token_list = NULL;
	heredoc_flag = 0;
	subshell_count = 0;
	heredoc_count = 0;
	// lexerでトークンリストを取得
	UNITY_TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, lexer((char *)input,
			&token_list), line_num, "TEST_01");
	// 文法チェックを実行
	while (token_list)
	{
		if (((t_token *)(token_list->content))->type == OP_HEREDOC)
			heredoc_flag = 1;
		result = check_tokens_grammar(&token_list, &subshell_count);
		if (result == OK && heredoc_flag == 1)
			heredoc_count++;
		heredoc_flag = 0;
		if (result == NG)
			break ;
		if (((t_token *)(token_list->content))->type == TERMINATOR)
			break ;
	}
	// 合格orエラーを検証
	UNITY_TEST_ASSERT_EQUAL_INT(expected_result, result, line_num,
		"TEST_02_OK/NG");
	// エラートークンの検証
	UNITY_TEST_ASSERT_NOT_NULL(token_list, line_num, "TEST_03");
	error_token = (t_token *)(token_list->content);
	if (error_token->type == TERMINATOR)
		err_text = "newline";
	else if (error_token->type == OPERAND_TEXT)
		err_text = replase_first_ifs_with_null(error_token->value);
	else
		err_text = error_token->value;
	UNITY_TEST_ASSERT_NOT_NULL(error_token, line_num, "TEST_04");
	UNITY_TEST_ASSERT_EQUAL_UINT(expected_id, error_token->id, line_num,
		"TEST_05_token_id");
	UNITY_TEST_ASSERT_EQUAL_INT(expected_type, error_token->type, line_num,
		"TEST_06_token_type");
	UNITY_TEST_ASSERT_EQUAL_STRING(expected_value, err_text, line_num,
		"TEST_07_token_value");
	UNITY_TEST_ASSERT_EQUAL_INT(expected_heredoc_count, heredoc_count, line_num,
		"TEST_08_heredoc_count");
	// クリーンアップ
	ft_lstclear(&token_list, free_token);
}

static void	test_grammar2(int expected_result, const char *input,
		int expected_id, t_token_type expected_type, const char *expected_value,
		int expected_heredoc_count, int line_num)
{
	t_list	*token_list;
	int		subshell_count;
	int		result;
	t_token	*error_token;
	char	*err_text;
	int		heredoc_count;
	int		heredoc_flag;

	// printf("line_num: %d\n", line_num);
	// char	*line_num_str;
	token_list = NULL;
	heredoc_flag = 0;
	subshell_count = 0;
	heredoc_count = 0;
	// lexerでトークンリストを取得
	UNITY_TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, lexer((char *)input,
			&token_list), line_num, "TEST_01");
	// 文法チェックを実行
	while (token_list)
	{
		if (((t_token *)(token_list->content))->type == OP_HEREDOC)
			heredoc_flag = 1;
		result = check_tokens_grammar(&token_list, &subshell_count);
		if (result == OK && heredoc_flag == 1)
			heredoc_count++;
		heredoc_flag = 0;
		if (result == NG)
			break ;
		if (((t_token *)(token_list->content))->type == TERMINATOR)
			break ;
	}
	// 合格orエラーを検証
	UNITY_TEST_ASSERT_EQUAL_INT(expected_result, result, line_num,
		"TEST_02_OK/NG");
	// エラートークンの検証
	UNITY_TEST_ASSERT_NOT_NULL(token_list, line_num, "TEST_03");
	error_token = (t_token *)(token_list->content);
	if (error_token->type == TERMINATOR)
		err_text = "newline";
	else if (error_token->type == OPERAND_TEXT)
		err_text = replase_first_ifs_with_null(error_token->value);
	else
		err_text = error_token->value;
	UNITY_TEST_ASSERT_NOT_NULL(error_token, line_num, "TEST_04");
	UNITY_TEST_ASSERT_EQUAL_UINT(expected_id, error_token->id, line_num,
		"TEST_05_token_id");
	UNITY_TEST_ASSERT_EQUAL_INT(expected_type, error_token->type, line_num,
		"TEST_06_token_type");
	UNITY_TEST_ASSERT_EQUAL_STRING(expected_value, err_text, line_num,
		"TEST_07_token_value");
	UNITY_TEST_ASSERT_EQUAL_INT(expected_heredoc_count, heredoc_count, line_num,
		"TEST_08_heredoc_count");
	// クリーンアップ
	ft_lstclear(&token_list, free_token);
}

/* ************************************************************************** */
// test functions
/* ************************************************************************** */
/* コマンドのテスト */

void	test_Command(void)
{
	test_grammar(OK, "echo hello", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "ls -la", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo hello <<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo hello >>", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo hello &&", 2, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo hello ||", 2, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo hello |", 2, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo hello <", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo hello >", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo hello (", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo hello )", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo hello '", 2, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo hello \"", 2, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	// test_grammar(NG,"| \n", 0, OP_PIPE, "|",0, __LINE__);
	test_grammar(OK, "echo hello echo a", 1, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo hello<<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo hello>>", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo hello&&", 2, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo hello||", 2, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo hello|", 2, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo hello<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo hello>", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo hello(", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo hello)", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo hello'", 2, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo hello\"", 2, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	// test_grammar(NG,"| \n", 0, OP_PIPE, "|",0, __LINE__);
	test_grammar(OK, "echo helloecho a", 1, TERMINATOR, "newline", 0, __LINE__);
	// エクストラケース
	test_grammar(OK, "bash echo bb", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "bash \"echo bb\"", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "sss && echo bbb", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo aaa < echo bbb", 3, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "echo echo b", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo $?", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "ls -a ~/", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo \"<\"", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo \"'a'\"", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo \"(\"", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "a=1", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo $SHLVL", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo aaa>echo bbb > echo ccc", 5, TERMINATOR, "newline",
		0, __LINE__);
	test_grammar(OK, "echo aaa > echo bbb", 3, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo $(echo b)", 1, OP_OPEN, "(", 0, __LINE__);
}

/* ************************************************************************** */
/* パイプのテスト */

void	test_Pipe(void)
{
	test_grammar(OK, "echo hello | grep h", 3, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "ls | grep a | wc -l", 5, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "ls | ./minishell | wc -l", 5, TERMINATOR, "newline", 0,
		__LINE__);
	// レギュラーのテスト ---start---
	test_grammar(NG, "|", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "| <<", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "| >>", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "| &&", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "| ||", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "| |", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "| <", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "| >", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "| (", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "| )", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "| '", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "| \"", 0, OP_PIPE, "|", 0, __LINE__);
	// test_grammar(NG,"| \n", 0, OP_PIPE, "|",0, __LINE__);
	test_grammar(NG, "| echo a", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "|<<", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "|>>", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "|&&", 0, OP_PIPE, "|", 0, __LINE__); // パイプ＆バックグラウンドは無し
	test_grammar(NG, "|||", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "||", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|<", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "|>", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "|(", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "|)", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "|'", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "|\"", 0, OP_PIPE, "|", 0, __LINE__);
	// test_grammar(NG,"|\n", 0, OP_PIPE, "||",0, __LINE__);
	test_grammar(NG, "|echo a", 0, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "echo a | <<", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a | >>", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a | &&", 2, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "echo a | ||", 2, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "echo a | |", 2, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "echo a | <", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a | >", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a | (", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a | )", 2, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo a | '", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a | \"", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	// test_grammar(NG,"| \n", 0, OP_PIPE, "|",0, __LINE__);
	test_grammar(OK, "echo a | echo a", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a |<<", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a |>>", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo a | &", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashでは&でNG
	test_grammar(NG, "echo a |&&", 2, OP_AND, "&&", 0, __LINE__);
	// bashでは&でNG
	test_grammar(NG, "echo a |||", 2, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "echo a ||", 2, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a |<", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a |>", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a |(", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a |)", 2, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo a |'", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a |\"", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	// test_grammar(NG,"| \n", 0, OP_PIPE, "|",0, __LINE__);
	test_grammar(OK, "echo a |echo a", 3, TERMINATOR, "newline", 0, __LINE__);
	// レギュラーのテスト ---end---
	// パイプの後にコマンドがない場合はエラー
	test_grammar(NG, "ls |", 2, TERMINATOR, "newline", 0, __LINE__);
	// パイプから始まる場合はエラー
	test_grammar(NG, "| ls", 0, OP_PIPE, "|", 0, __LINE__);
	// 連続したパイプはエラー
	test_grammar(NG, "ls | | grep a", 2, OP_PIPE, "|", 0, __LINE__);
	// パイプと論理演算子が隣接
	test_grammar(NG, "ls | && cat", 2, OP_AND, "&&", 0, __LINE__);
	// パイプの後に直接リ��イレクト
	test_grammar(OK, "ls | > file", 4, TERMINATOR, "newline", 0, __LINE__);
	// 特殊ケース
	// パイプの後にコマンドがない場合、パイプがエラートークン
	test_grammar(NG, "ls |", 2, TERMINATOR, "newline", 0, __LINE__);
	// パイプから始まる場合、パイプがエラートークン
	test_grammar(NG, "| ls", 0, OP_PIPE, "|", 0, __LINE__);
	// 連続したパイプの場合、2つ目のパイプがエラートークン
	test_grammar(NG, "ls | | grep", 2, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "ls | && cat", 2, OP_AND, "&&", 0, __LINE__);
	// エクストラケース
	test_grammar(OK, "bash echo $a | cat", 3, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "echo a | bash echo $a | cat", 5, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "echo a | bash echo a | cat", 5, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "echo \"echo aaa\" | bash", 7, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "echo \"\"echo $a\"\" | bash", 11, TERMINATOR, "newline",
		0, __LINE__);
	test_grammar(OK, "bash | echo bb | cat", 5, TERMINATOR, "newline", 0,
		__LINE__);
}

/* ************************************************************************** */
/* リダイレクトのテスト */

void	test_RedirectInput(void)
{
	test_grammar(NG, "<", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "cat < input.txt", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "< <<", 1, OP_HEREDOC, "<<", 0, __LINE__);
	test_grammar(NG, "< >>", 1, OP_APPEND, ">>", 0, __LINE__);
	test_grammar(NG, "< &&", 1, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "< ||", 1, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "< |", 1, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "< <", 1, OP_INPUT, "<", 0, __LINE__);
	test_grammar(NG, "< >", 1, OP_OUTPUT, ">", 0, __LINE__);
	test_grammar(NG, "< (", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "< )", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "< '", 1, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, "< \"", 1, QUOTE_DOUBLE, "\"", 0, __LINE__);
	// test_grammar(NG,"< \n", 0, OP_OR, "||",0, __LINE__);
	test_grammar(OK, "< echo a", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "<<<", 1, OP_INPUT, "<", 0, __LINE__); // bashではOK
	// test_grammar(NG, "<>>", 1, OP_OUTPUT, ">",0, __LINE__); // なぜか>が優先
	test_grammar(NG, "<&&", 1, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "<||", 1, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "<|", 1, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "<<", 1, TERMINATOR, "newline", 0, __LINE__);
	// test_grammar(NG, "<>", 2, TERMINATOR, "newline",0, __LINE__); // 特殊なケース
	test_grammar(NG, "<(", 1, OP_OPEN, "(", 0, __LINE__); // bashではOK プロセス置換え
	test_grammar(NG, "<)", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "<'", 1, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, "<\"", 1, QUOTE_DOUBLE, "\"", 0, __LINE__);
	// test_grammar(NG,"<\n", 0, OP_OR, "||",0, __LINE__);
	test_grammar(OK, "<echo a", 2, TERMINATOR, "newline", 0, __LINE__);
}

void	test_RedirectOutput(void)
{
	test_grammar(NG, ">", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "ls > output.txt", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "> <<", 1, OP_HEREDOC, "<<", 0, __LINE__);
	test_grammar(NG, "> >>", 1, OP_APPEND, ">>", 0, __LINE__);
	test_grammar(NG, "> &&", 1, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "> ||", 1, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "> |", 1, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "> <", 1, OP_INPUT, "<", 0, __LINE__);
	test_grammar(NG, "> >", 1, OP_OUTPUT, ">", 0, __LINE__);
	test_grammar(NG, "> (", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "> )", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "> '", 1, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, "> \"", 1, QUOTE_DOUBLE, "\"", 0, __LINE__);
	// test_grammar(NG, "> \n", 0, OP_OR, "||",0, __LINE__);
	test_grammar(OK, "> echo a", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "><<", 1, OP_HEREDOC, "<<", 0, __LINE__);
	test_grammar(NG, ">>>", 1, OP_OUTPUT, ">", 0, __LINE__);
	test_grammar(NG, ">&&", 1, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, ">||", 1, OP_OR, "||", 0, __LINE__); // パイプ＆バックグラウンドは無し
	test_grammar(NG, ">|", 1, OP_PIPE, "|", 0, __LINE__); // パイプ＆バックグラウンドは無し
	test_grammar(NG, "><", 1, OP_INPUT, "<", 0, __LINE__);
	test_grammar(NG, ">>", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, ">(", 1, OP_OPEN, "(", 0, __LINE__); // bashではOK プロセス置換え
	test_grammar(NG, ">)", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ">'", 1, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, ">\"", 1, QUOTE_DOUBLE, "\"", 0, __LINE__);
	// test_grammar(NG, ">\n", 0, OP_OR, "||",0, __LINE__);
	test_grammar(OK, ">echo a", 2, TERMINATOR, "newline", 0, __LINE__);
	// リダ�����レクトの後にファイル名がない場合はエラー
	test_grammar(NG, "ls >", 2, TERMINATOR, "newline", 0, __LINE__);
	// リダイレクトから始まる場合はエラー
	test_grammar(OK, "> output.txt", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "ls > > file", 2, OP_OUTPUT, ">", 0, __LINE__);
}

void	test_RedirectAppend(void)
{
	test_grammar(NG, ">>", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo hello >> log.txt", 3, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, ">> <<", 1, OP_HEREDOC, "<<", 0, __LINE__);
	test_grammar(NG, ">> >>", 1, OP_APPEND, ">>", 0, __LINE__);
	test_grammar(NG, ">> &&", 1, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, ">> ||", 1, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, ">> |", 1, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, ">> <", 1, OP_INPUT, "<", 0, __LINE__);
	test_grammar(NG, ">> >", 1, OP_OUTPUT, ">", 0, __LINE__);
	test_grammar(NG, ">> (", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, ">> )", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ">> '", 1, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, ">> \"", 1, QUOTE_DOUBLE, "\"", 0, __LINE__);
	// test_grammar(NG, ">> \n", 0, OP_OR, "||",0, __LINE__);
	test_grammar(OK, ">> echo a", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, ">><<", 1, OP_HEREDOC, "<<", 0, __LINE__);
	test_grammar(NG, ">>>>", 1, OP_APPEND, ">>", 0, __LINE__);
	test_grammar(NG, ">>&&", 1, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, ">>||", 1, OP_OR, "||", 0, __LINE__); // パイプ＆バックグラウンドは無し
	test_grammar(NG, ">>|", 1, OP_PIPE, "|", 0, __LINE__); // パイプ＆バックグラウンドは無し
	test_grammar(NG, ">><", 1, OP_INPUT, "<", 0, __LINE__);
	test_grammar(NG, ">>>", 1, OP_OUTPUT, ">", 0, __LINE__);
	test_grammar(NG, ">>(", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, ">>)", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ">>'", 1, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, ">>\"", 1, QUOTE_DOUBLE, "\"", 0, __LINE__);
	// test_grammar(NG, ">>\n", 0, OP_OR, "||",0, __LINE__);
	test_grammar(OK, ">>echo a", 2, TERMINATOR, "newline", 0, __LINE__);
}

void	test_RedirectHeredoc(void)
{
	test_grammar(NG, "<<", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "<< <<", 1, OP_HEREDOC, "<<", 0, __LINE__);
	test_grammar(NG, "<< >>", 1, OP_APPEND, ">>", 0, __LINE__);
	test_grammar(NG, "<< &&", 1, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "<< ||", 1, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "<< |", 1, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "<< <", 1, OP_INPUT, "<", 0, __LINE__);
	test_grammar(NG, "<< >", 1, OP_OUTPUT, ">", 0, __LINE__);
	test_grammar(NG, "<< (", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "<< )", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "<< '", 1, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, "<< \"", 1, QUOTE_DOUBLE, "\"", 0, __LINE__);
	// test_grammar(NG,"< \n", 0, OP_OR, "||",0, __LINE__);
	test_grammar(OK, "<< echo a", 2, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(NG, "<<<<", 1, OP_HEREDOC, "<<", 0, __LINE__);
	test_grammar(NG, "<<>>", 1, OP_APPEND, ">>", 0, __LINE__);
	test_grammar(NG, "<<&&", 1, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "<<||", 1, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "<<|", 1, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "<<<", 1, OP_INPUT, "<", 0, __LINE__);
	test_grammar(NG, "<<>", 1, OP_OUTPUT, ">", 0, __LINE__);
	test_grammar(NG, "<<(", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "<<)", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "<<'", 1, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, "<<\"", 1, QUOTE_DOUBLE, "\"", 0, __LINE__);
	// test_grammar(NG,"<\n", 0, OP_OR, "||",0, __LINE__);
	test_grammar(OK, "<<echo a", 2, TERMINATOR, "newline", 1, __LINE__);
	// エクストラケース
	test_grammar(OK, "<<\\", 2, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(OK, "echo a | echo b && out << end", 7, TERMINATOR, "newline",
		1, __LINE__);
	test_grammar(OK, "<<end aa", 2, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(OK, "<<\\n", 2, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(OK, "<<\"\"", 4, TERMINATOR, "newline", 1, __LINE__);
	// テキストかクオーとブロックが続く限り、検査を続ける
	test_grammar(OK, "<<\"\"aa", 5, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(OK, "<<'\"'", 4, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(OK, "cat << $USER", 3, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(OK, "cat <<aa", 3, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(OK, "cat <<\"aa\"", 5, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(OK, "cat <<'aa'", 5, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(OK, "echo a && << a", 4, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(OK, "bash << end", 3, TERMINATOR, "newline", 1, __LINE__);
	test_grammar(NG, "cat (<< ED < out)", 1, OP_OPEN, "(", 0, __LINE__);
	//シェル関数には対応しないのでbashとは異なる挙動にする
	test_grammar(NG, "< << a", 1, OP_HEREDOC, "<<", 0, __LINE__);
	// 途中で止めるケース
	test_grammar(NG, "<<\\(\\)", 2, OP_OPEN, "(", 1, __LINE__);
	test_grammar(NG, "(<<\\))", 4, OP_CLOSE, ")", 1, __LINE__);
	test_grammar(OK, "echo a | echo b && cat << end > out", 9, TERMINATOR,
		"newline", 1, __LINE__);
	test_grammar(OK, "((cat << ED < out)| << end)", 12, TERMINATOR, "newline",
		2, __LINE__);
	// test_grammar(OK, "< out)| << end)", 5, OP_CLOSE, ")",0, __LINE__);
	test_grammar(OK, "<< a << b << c", 6, TERMINATOR, "newline", 3, __LINE__);
	test_grammar(OK, "cat << end | (cat << abc)", 9, TERMINATOR, "newline", 2,
		__LINE__);
}

void	test_RedirectCombined(void)
{
	test_grammar(OK, "cat < input.txt > output.txt", 5, TERMINATOR, "newline",
		0, __LINE__);
}

/* ************************************************************************** */
/* 論理演算子のテスト */

void	test_AndOperator(void)
{
	test_grammar(OK, "ls && echo success", 3, TERMINATOR, "newline", 0,
		__LINE__);
	// レギュラーのテスト ---start---
	test_grammar(NG, "&&", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&& <<", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&& >>", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&& &&", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&& ||", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&& |", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&& <", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&& >", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&& (", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&& )", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&& '", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&& \"", 0, OP_AND, "&&", 0, __LINE__);
	// test_grammar(NG,"&& \n", 0, OP_AND,  "&&",0, __LINE__);
	test_grammar(NG, "&& echo a", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&&<<", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&&>>", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&&&&", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&&||", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&&|", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&&<", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&&>", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&&(", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&&)", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&&'", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "&&\"", 0, OP_AND, "&&", 0, __LINE__);
	// test_grammar(NG,"&&\n", 0, OP_AND,  "&&",0, __LINE__);
	test_grammar(NG, "&&echo a", 0, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "echo a && <<", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a && >>", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a && &&", 2, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "echo a && ||", 2, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "echo a && |", 2, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "echo a && <", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a && >", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a && (", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a && )", 2, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo a && '", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a && \"", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	// test_grammar(NG,"&& \n", 0, OP_AND,  "&&",0, __LINE__);
	test_grammar(OK, "echo a && echo a", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a &&<<", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a &&>>", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a &&&&", 2, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "echo a &&||", 2, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "echo a &&|", 2, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "echo a &&<", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a &&>", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a &&(", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a &&)", 2, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo a &&'", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a &&\"", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashで���OK
	// test_grammar(NG,"&& \n", 0, OP_AND,  "&&",0, __LINE__);
	test_grammar(OK, "echo a &&echo a", 3, TERMINATOR, "newline", 0, __LINE__);
	// レギュラーのテスト ---end---
	// ANDの�������������������������������にコマンドがない場合はエラー
	test_grammar(NG, "ls &&", 2, TERMINATOR, "newline", 0, __LINE__);
	// ANDから始まる場合はエラー
	test_grammar(NG, "&& ls", 0, OP_AND, "&&", 0, __LINE__);
	// "&&" と "||" のみの入力では最初の "&&" がエラートークンとして返される
	test_grammar(NG, "&& ||", 0, OP_AND, "&&", 0, __LINE__);
	// "echo aaa && ||" の場合は "||" がエラートークンとして返される
	test_grammar(NG, "echo aaa && ||", 2, OP_OR, "||", 0, __LINE__);
	// AND演算子の後にコマンドがない場合、ANDがエラートークン
	test_grammar(NG, "ls &&", 2, TERMINATOR, "newline", 0, __LINE__);
}

void	test_OrOperator(void)
{
	test_grammar(OK, "ls || echo failure", 3, TERMINATOR, "newline", 0,
		__LINE__);
	// レギュラーのテスト ---start---
	test_grammar(NG, "||", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|| <<", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|| >>", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|| &&", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|| ||", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|| |", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|| <", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|| >", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|| (", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|| )", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|| '", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|| \"", 0, OP_OR, "||", 0, __LINE__);
	// test_grammar(NG,"|| \n", 0, OP_OR, "||",0, __LINE__);
	test_grammar(NG, "|| echo a", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "||<<", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "||>>", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "||&&", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "||||", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "|||", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "||<", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "||>", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "||(", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "||)", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "||'", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "||\"", 0, OP_OR, "||", 0, __LINE__);
	// test_grammar(NG,"||\n", 0, OP_OR, "||",0, __LINE__);
	test_grammar(NG, "||echo a", 0, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "echo a || <<", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a || >>", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a || &&", 2, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "echo a || ||", 2, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "echo a || |", 2, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "echo a || <", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a || >", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a || (", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a || )", 2, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo a || '", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a || \"", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	// test_grammar(NG,"&& \n", 0, OP_AND,  "&&",0, __LINE__);
	test_grammar(OK, "echo a || echo a", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a ||<<", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a ||>>", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a ||&&", 2, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "echo a ||||", 2, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "echo a |||", 2, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "echo a ||<", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a ||>", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo a ||(", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a ||)", 2, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo a ||'", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "echo a ||\"", 3, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	// test_grammar(NG,"&& \n", 0, OP_AND,  "&&",0, __LINE__);
	test_grammar(OK, "echo a ||echo a", 3, TERMINATOR, "newline", 0, __LINE__);
	// レギュラーのテスト ---end---
	// ORの後にコマンドがない場合はエラー
	test_grammar(NG, "ls ||", 2, TERMINATOR, "newline", 0, __LINE__);
	// ORから始まる場合はエラー
	test_grammar(NG, "|| ls", 0, OP_OR, "||", 0, __LINE__);
	// OR演算子の後にコマンドがない場��、OR��エラートークン
	test_grammar(NG, "ls ||", 2, TERMINATOR, "newline", 0, __LINE__);
}

void	test_AndOrCombined(void)
{
	test_grammar(OK, "ls && echo success || echo failure", 5, TERMINATOR,
		"newline", 0, __LINE__);
}

/* ************************************************************************** */
/* サブシェルのテスト */

void	test_SimpleSubshell(void)
{
	test_grammar(OK, "(ls)", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "((ls))", 5, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "(((ls)))", 7, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "(cat output.txt)", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "(echo a)", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "(export a=2)", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "(ls > out.txt)", 5, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "((echo a) > echo)", 7, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "(ls && (echo hello))", 7, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "(ls) | grep a", 5, TERMINATOR, "newline", 0, __LINE__);
	// レギュラーのテスト ---start---
	// NG open
	test_grammar(NG, "(", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "( <<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "( >>", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "( &&", 1, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "( ||", 1, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "( |", 1, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "( <", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "( >", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "( (", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "( )", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "( '", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "( \"", 2, TERMINATOR, "newline", 0, __LINE__);
	// test_grammar(NG, "( \n", 2, TERMINATOR, "newline",0, __LINE__);
	test_grammar(NG, "( echo a", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(<<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(>>", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(&&", 1, OP_AND, "&&", 0, __LINE__);
	test_grammar(NG, "(||", 1, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "(|", 1, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "(<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(>", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "((", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "()", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "('", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(\"", 2, TERMINATOR, "newline", 0, __LINE__);
	// test_grammar(NG, "(\n", 2, TERMINATOR, "newline",0, __LINE__);
	test_grammar(NG, "(echo a", 2, TERMINATOR, "newline", 0, __LINE__);
	// NG close
	test_grammar(NG, ")", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ") <<", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ") >>", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ") &&", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ") ||", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ") |", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ") <", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ") >", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ") (", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ") )", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ") '", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ") \"", 0, OP_CLOSE, ")", 0, __LINE__);
	// test_grammar(NG, ") \n", 0, OP_CLOSE, ")",0, __LINE__);
	test_grammar(NG, ") echo a", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ")<<", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ")>>", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ")&&", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ")||", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ")|", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ")<", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ")>", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ")(", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "))", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ")'", 0, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, ")\"", 0, OP_CLOSE, ")", 0, __LINE__);
	// test_grammar(NG, ")\n", 0, OP_CLOSE, ")",0, __LINE__);
	test_grammar(NG, ")echo a", 0, OP_CLOSE, ")", 0, __LINE__);
	// NG open close
	test_grammar(NG, "(echo a) <<", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(echo a) >>", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(echo a) &&", 4, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "(echo a) ||", 4, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "(echo a) |", 4, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "(echo a) <", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(echo a) >", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(echo a) (", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a) )", 3, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "(echo a) '", 3, QUOTE_SINGLE, "'", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "(echo a) \"", 3, QUOTE_DOUBLE, "\"", 0, __LINE__);
	// bashではOK
	// test_grammar(NG, "(echo a) \n", 0, OP_CLOSE, ")",0, __LINE__);
	test_grammar(NG, "(echo a) echo a", 3, OPERAND_TEXT, "echo", 0, __LINE__);
	test_grammar(NG, "(echo a)<<", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(echo a)>>", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(echo a)&&", 4, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "(echo a)||", 4, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "(echo a)|", 4, TERMINATOR, "newline", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "(echo a)<", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(echo a)>", 4, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "(echo a)(", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a))", 3, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "(echo a)'", 3, QUOTE_SINGLE, "'", 0, __LINE__);
	// bashではOK
	test_grammar(NG, "(echo a)\"", 3, QUOTE_DOUBLE, "\"", 0, __LINE__);
	// bashではOK
	// test_grammar(NG, "(echo a) \n", 0, OP_CLOSE, ")",0, __LINE__);
	test_grammar(NG, "(echo a)echo a", 3, OPERAND_TEXT, "echo", 0, __LINE__);
	// レギュラーのテスト ---end---
	// 開きカッコだけの場合はエ�����������ー
	test_grammar(NG, "(ls", 2, TERMINATOR, "newline", 0, __LINE__);
	// 閉じカッコだけの場合はエラー
	test_grammar(NG, "ls)", 1, OP_CLOSE, ")", 0, __LINE__);
	// 空のサブシェルはエラー
	test_grammar(NG, "()", 1, OP_CLOSE, ")", 0, __LINE__);
	// 不完全なサブシェル内の式
	test_grammar(NG, "(ls &&) || echo", 3, OP_CLOSE, ")", 0, __LINE__);
	// 無効なサブシェルの使用
	test_grammar(NG, "ls (cat file)", 1, OP_OPEN, "(", 0, __LINE__);
	// 開きカッコのみの場合、終端のトークンがエラートークン（閉じカッコがない）
	test_grammar(NG, "(ls", 2, TERMINATOR, "newline", 0, __LINE__);
	// 閉じカッコが余分にある場合、閉じカッコがエラートークン
	test_grammar(NG, "ls)", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "(ls &&) || echo", 3, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "ls (cat file)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(ls | | wc)", 3, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "(echo && || cat)", 3, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "(echo > )", 3, OP_CLOSE, ")", 0, __LINE__);
	// 入れ子になったサブシェル内のエラー
	test_grammar(NG, "(ls && (echo | | cat))", 6, OP_PIPE, "|", 0, __LINE__);
	test_grammar(NG, "(ls && (echo &&) || cat)", 6, OP_CLOSE, ")", 0, __LINE__);
	// エクストラケース
	test_grammar(NG, "(ls && (echo hello)", 6, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "cat \"\" (echo aaa)", 5, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "cat \"\"(echo aaa)", 4, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo aaa (echo aaa)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "ls -A (echo aaa)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo)", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo )", 1, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "export (=1", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo echo) (echo b) ", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(ls && echo hello || | grep pattern) ", 5, OP_PIPE, "|",
		0, __LINE__);
	test_grammar(OK, "( (ls) )", 5, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "((ls))", 5, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "( (echo a | cat ) )", 7, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "((echo a | cat ))", 7, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "(ls && (echo hello) && (wc -l))", 11, TERMINATOR,
		"newline", 0, __LINE__);
	// bashではNG
	test_grammar(NG, "((echo a) echo b )", 4, OPERAND_TEXT, "echo", 0,
		__LINE__);
	test_grammar(NG, "((echo a)\\n echo b)", 4, OPERAND_TEXT, "\\n", 0,
		__LINE__);
	test_grammar(NG, "((echo a)\"\\n\" echo b)", 4, QUOTE_DOUBLE, "\"", 0,
		__LINE__);
	test_grammar(NG, "((echo a)\"abc\" echo b)", 4, QUOTE_DOUBLE, "\"", 0,
		__LINE__);
	test_grammar(NG, "((echo a)\"abc) echo b", 4, QUOTE_DOUBLE, "\"", 0,
		__LINE__);
	//独自ケース
	test_grammar(NG, "((echo a)\"abc)", 4, QUOTE_DOUBLE, "\"", 0, __LINE__);
	test_grammar(NG, "((echo a)abc\")", 4, OPERAND_TEXT, "abc", 0, __LINE__);
	test_grammar(NG, "((echo a)'\\n' echo b)", 4, QUOTE_SINGLE, "'", 0,
		__LINE__);
	test_grammar(NG, "((echo a)'abc' echo b)", 4, QUOTE_SINGLE, "'", 0,
		__LINE__);
	test_grammar(NG, "((echo a)'abc) echo b", 4, QUOTE_SINGLE, "'", 0,
		__LINE__);
	//独自ケース
	test_grammar(NG, "((echo a)'abc)", 4, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, "((echo a)abc')", 4, OPERAND_TEXT, "abc", 0, __LINE__);
	test_grammar(NG, "((echo a>aaa)echo b)", 6, OPERAND_TEXT, "echo", 0,
		__LINE__);
	test_grammar(NG, "(echo a>aaa)(echo b)", 5, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a>aaa s)(echo b)", 5, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a ( | echo b) )", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a(echo b ))", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cd /tmp && touch file.txt(ls > output.txt ))", 4,
		OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "((ls > output.txt)(cd /tmp && touch file.txt))", 6,
		OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "((ls)(ls))", 4, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo $a) | cat)", 5, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "((echo a)<<< \"\\n\" echo b)", 5, OP_INPUT, "<", 0,
		__LINE__);
	test_grammar(NG, "((env | cat) cat)", 6, OPERAND_TEXT, "cat", 0, __LINE__);
	test_grammar(NG, "((echo a;)echo b)", 4, OPERAND_TEXT, "echo", 0, __LINE__);
	test_grammar(NG, "((ls > output.txt)cd /tmp && touch file.txt)", 6,
		OPERAND_TEXT, "cd", 0, __LINE__);
	test_grammar(NG, "((echo a | cat) | cat ) ; echo $?", 9, OPERAND_TEXT, ";",
		0, __LINE__); // bashではOK
	test_grammar(NG, "(echo a;(echo b;);echo c;)", 2, OP_OPEN, "(", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "((echo a;);echo b;)", 4, OPERAND_TEXT, ";echo", 0,
					__LINE__); // bashではOK
	test_grammar(OK, "(dada) || (echo bbb)", 7, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "(dada) && (echo bbb)", 7, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "((echo a | cat) | cat )", 9, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "((echo a)|echo b)", 7, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "(env | cat)", 5, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "((env | cat) | cat)", 9, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "(((env | cat)  | cat ) | cat)", 13, TERMINATOR, "newline",
		0, __LINE__);
	test_grammar(OK, "((echo $a) | cat)", 7, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "(echo a>echo b)", 5, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "(echo a | cat) > aaa", 7, TERMINATOR, "newline", 0,
		__LINE__);
	// シェル関数に対応��ないのでbashとは挙動を変更するパターン
	test_grammar(NG, "echo (", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo ()", 1, OP_OPEN, "(", 0, __LINE__); // bashではOK
	test_grammar(NG, "echo (echo aaa)", 1, OP_OPEN, "(", 0, __LINE__);
	// コマンド置換えに対応しないのでbash����は挙動を変更するパターン
	test_grammar(OK, "\"$(ls)\"", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "\"$(echo aaa)\"", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "echo $(echo aaa)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "($(echo a) $(echo b) )", 2, OP_OPEN, "(", 0, __LINE__);
	//  サブシェルの後のリダイレクト先は厳密に単語を解釈する
	test_grammar(OK, "((echo a)>echo )", 7, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "((echo a)>echo b)", 5, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(NG, "( (echo a)>echo b)", 5, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(NG, "(echo a)>echo b", 4, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(NG, "( (echo a)>echo b", 5, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(OK, "echo a>echo b", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "((echo a)>||echo b)", 5, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "((echo a)>'echo b)", 5, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, "((echo a)>\"echo b)", 5, QUOTE_DOUBLE, "\"", 0, __LINE__);
	test_grammar(NG, "( > a (> b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "((echo a)>'echo b' c)", 8, OPERAND_TEXT, "c", 0,
		__LINE__);
	test_grammar(OK, "((echo a)>'echo b')", 9, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "((echo a)>\"echo b\")", 9, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "((echo a)>echo)", 7, TERMINATOR, "newline", 0, __LINE__);
	//
	test_grammar(NG, "((echo a)<echo b)", 5, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(NG, "( (echo a)<echo b)", 5, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(NG, "(echo a)<echo b", 4, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(NG, "( (echo a)<echo b", 5, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(OK, "echo a<echo b", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "((echo a)<||echo b)", 5, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "((echo a)<'echo b)", 5, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, "((echo a)<\"echo b)", 5, QUOTE_DOUBLE, "\"", 0, __LINE__);
	test_grammar(NG, "( < a (< b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "((echo a)<'echo b' c)", 8, OPERAND_TEXT, "c", 0,
		__LINE__);
	test_grammar(OK, "((echo a)<'echo b')", 9, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "((echo a)<\"echo b\")", 9, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "((echo a)<echo)", 7, TERMINATOR, "newline", 0, __LINE__);
	//
	test_grammar(NG, "((echo a)>>echo b)", 5, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(NG, "( (echo a)>>echo b)", 5, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(NG, "(echo a)>>echo b", 4, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(NG, "( (echo a)>>echo b", 5, OPERAND_TEXT, "b", 0, __LINE__);
	test_grammar(OK, "echo a>>echo b", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "((echo a)>>||echo b)", 5, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "((echo a)>>'echo b)", 5, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, "((echo a)>>\"echo b)", 5, QUOTE_DOUBLE, "\"", 0,
		__LINE__);
	test_grammar(NG, "( >> a (>> b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "((echo a)>>'echo b' c)", 8, OPERAND_TEXT, "c", 0,
		__LINE__);
	test_grammar(OK, "((echo a)>>'echo b')", 9, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "((echo a)>>\"echo b\")", 9, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "((echo a)>>echo)", 7, TERMINATOR, "newline", 0, __LINE__);
	// サブシェルの後のリダイレクト先は厳密に単語を解釈する
	test_grammar(NG, "((echo a)<<||echo b)", 5, OP_OR, "||", 0, __LINE__);
	test_grammar(NG, "((echo a)<<'echo b)", 5, QUOTE_SINGLE, "'", 0, __LINE__);
	test_grammar(NG, "((echo a)<<\"echo b)", 5, QUOTE_DOUBLE, "\"", 0,
		__LINE__);
	// エラー出してからheredoc実行
	test_grammar(NG, "((echo a)<<echo b)", 5, OPERAND_TEXT, "b", 1, __LINE__);
	//
	test_grammar(NG, "( (echo a)<<echo b)", 5, OPERAND_TEXT, "b", 1, __LINE__);
	//
	test_grammar(NG, "( << a (<< b))", 3, OP_OPEN, "(", 1, __LINE__);
	// heredocを実行してからエラーを出す
	test_grammar(NG, "(echo a)<<echo b", 4, OPERAND_TEXT, "b", 1, __LINE__);
	//
	test_grammar(NG, "( (echo a)<<echo b", 5, OPERAND_TEXT, "b", 1, __LINE__);
	//
	test_grammar(NG, "((echo a)<<'echo b' c)", 8, OPERAND_TEXT, "c", 1,
		__LINE__);
	// 正常実行
	test_grammar(OK, "echo a<<echo b", 3, TERMINATOR, "newline", 1, __LINE__);
	//
	test_grammar(OK, "((echo a)<<'echo b')", 9, TERMINATOR, "newline", 1,
		__LINE__);
	test_grammar(OK, "((echo a)<<\"echo b\")", 9, TERMINATOR, "newline", 1,
		__LINE__);
	test_grammar(OK, "((echo a)<<echo)", 7, TERMINATOR, "newline", 1, __LINE__);
	//
	//  プロセス置換えに対応しないのでbashとは挙動を変更するパターン
	test_grammar(NG, "<(ls)", 1, OP_OPEN, "(", 0, __LINE__);
	// input
	test_grammar(NG, "(cat<(echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat<(echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "cat<(echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat<(echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat< (echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat< (echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "cat< (echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat< (echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	//
	test_grammar(NG, "(echo a<(echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a<(echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo a<(echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a<(echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a< (echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a< (echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo a< (echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a< (echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "<(echo b)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "< (echo b)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(OK, "< echo b", 2, TERMINATOR, "newline", 0, __LINE__);
	// output
	test_grammar(NG, "(cat>(echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat>(echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "cat>(echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat>(echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat> (echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat> (echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "cat> (echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat> (echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	//
	test_grammar(NG, "(echo a>(echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a>(echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo a>(echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a>(echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a> (echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a> (echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo a> (echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a> (echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, ">(echo b)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "> (echo b)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(OK, "> echo b", 2, TERMINATOR, "newline", 0, __LINE__);
	// append
	test_grammar(NG, "(cat>>(echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat>>(echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "cat>>(echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat>>(echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat>> (echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat>> (echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "cat>> (echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat>> (echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	//
	test_grammar(NG, "(echo a>>(echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a>>(echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo a>>(echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a>>(echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a>> (echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a>> (echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo a>> (echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a>> (echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, ">>(echo b)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, ">> (echo b)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(OK, ">> echo b", 2, TERMINATOR, "newline", 0, __LINE__);
	// heredoc
	test_grammar(NG, "(cat<<(echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat<<(echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "cat<<(echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat<<(echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat<< (echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat<< (echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "cat<< (echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(cat<< (echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	//
	test_grammar(NG, "(echo a<<(echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a<<(echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo a<<(echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a<<(echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a<< (echo b))", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a<< (echo b) )", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo a<< (echo b)", 2, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "(echo a<< (echo b)", 3, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "<<(echo b)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "<< (echo b)", 1, OP_OPEN, "(", 0, __LINE__);
	test_grammar(OK, "<< echo b", 2, TERMINATOR, "newline", 1, __LINE__);
}

/* ************************************************************************** */
/* クォートのテスト */

void	test_SingleQuotes(void)
{
	test_grammar(NG, "'", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo 'hello world'", 4, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "echo 'hello \"world\"'", 4, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo 'hello world' <<", 6, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo 'hello world' >>", 6, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo 'hello world' &&", 6, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo 'hello world' ||", 6, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo 'hello world' |", 6, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo 'hello world' <", 6, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo 'hello world' >", 6, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo 'hello world' (", 5, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo 'hello world' )", 5, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo 'hello world' '", 6, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo 'hello world' \"", 6, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo 'hello world'<<", 5, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo 'hello world'>>", 5, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo 'hello world'&&", 5, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo 'hello world'||", 5, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo 'hello world'|", 5, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo 'hello world'<", 5, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo 'hello world'>", 5, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo 'hello world'(", 4, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo 'hello world')", 4, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo 'hello world''", 5, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo 'hello world'\"", 5, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "'hello world <<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "' <<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "' >>", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "' &&", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "' ||", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "' |", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "' <", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "' >", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "' (", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "' )", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "' '", 3, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "' \"", 2, TERMINATOR, "newline", 0, __LINE__);
	// test_grammar(NG, "' \n", 2, TERMINATOR, "newline",0, __LINE__);
	test_grammar(NG, "' echo a", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "'<<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "'>>", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "'&&", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "'||", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "'|", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "'<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "'>", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "'(", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "')", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "''", 3, TERMINATOR, "newline", 0, __LINE__); //空文字が挿入される
	test_grammar(NG, "'\"", 2, TERMINATOR, "newline", 0, __LINE__);
	// test_grammar(NG, "'\n", 2, TERMINATOR, "newline",0, __LINE__);
	test_grammar(NG, "'echo a", 2, TERMINATOR, "newline", 0, __LINE__);
}

void	test_DoubleQuotes(void)
{
	test_grammar(NG, "\"", 1, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo \"hello world\"", 4, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(OK, "echo \"hello 'world'\"", 4, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo \"hello world\" <<", 6, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo \"hello world\" >>", 6, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo \"hello world\" &&", 6, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo \"hello world\" ||", 6, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo \"hello world\" |", 6, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo \"hello world\" <", 6, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo \"hello world\" >", 6, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo \"hello world\" (", 5, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo \"hello world\" )", 5, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo \"hello world\" '", 6, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo \"hello world\" \"", 6, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo \"hello world\"<<", 5, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo \"hello world\">>", 5, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo \"hello world\"&&", 5, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo \"hello world\"||", 5, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo \"hello world\"|", 5, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo \"hello world\"<", 5, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo \"hello world\">", 5, TERMINATOR, "newline", 0,
		__LINE__);
	test_grammar(NG, "echo \"hello world\"(", 4, OP_OPEN, "(", 0, __LINE__);
	test_grammar(NG, "echo \"hello world\")", 4, OP_CLOSE, ")", 0, __LINE__);
	test_grammar(NG, "echo \"hello world\"'", 5, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "echo \"hello world\"\"", 5, TERMINATOR, "newline", 0,
					__LINE__); // bashではOK
	test_grammar(NG, "\"hello world <<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\" <<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\" >>", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\" &&", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\" ||", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\" |", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\" <", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\" >", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\" (", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\" )", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\" '", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "\" \"", 3, TERMINATOR, "newline", 0, __LINE__);
	// test_grammar(NG, "\" \n", 2, TERMINATOR, "newline",0, __LINE__);
	test_grammar(NG, "\" echo a", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\"<<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\">>", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\"&&", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\"||", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\"|", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\"<", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\">", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\"(", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\")", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(NG, "\"'", 2, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "\"\"", 3, TERMINATOR, "newline", 0, __LINE__);
	// test_grammar(NG, "\"\n", 2, TERMINATOR, "newline",0, __LINE__);
	test_grammar(NG, "\"echo a", 2, TERMINATOR, "newline", 0, __LINE__);
	// エクストラケース
	test_grammar(OK, "\"$echo aaa\"", 3, TERMINATOR, "newline", 0, __LINE__);
}

/* ************************************************************************** */
/* 複��演算子のテスト */

void	test_ComplexCommand1(void)
{
	char	*input;

	test_grammar(OK, "ls -la | grep a > output.txt", 5, TERMINATOR, "newline",
		0, __LINE__);
	test_grammar(OK, "(cat < input.txt) | grep a && echo success", 9,
		TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "ls && (echo success | grep s) || echo failure", 9,
		TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "(ls && echo success) || (echo failure && ls -la)", 11,
		TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "cat << EOF | grep pattern > output.txt", 7, TERMINATOR,
		"newline", 1, __LINE__);
	input = "ls -la | grep a > file && echo 'success' || echo \"failure\"";
	test_grammar(OK, input, 16, TERMINATOR, "newline", 0, __LINE__);
	input = "(cat < input.txt | grep pattern) > output.txt && echo done";
	test_grammar(OK, input, 11, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "cat << EOF | (grep pattern && wc -l) >> log.txt", 11,
		TERMINATOR, "newline", 1, __LINE__);
	input = "(ls -la > file) && (cat file | grep pattern)|| echo 'not found'";
	test_grammar(OK, input, 16, TERMINATOR, "newline", 0, __LINE__);
	test_grammar(OK, "echo \"start\" && (cat < file1 > file2) | wc -l", 15,
		TERMINATOR, "newline", 0, __LINE__);
}

//
/* ************************************************************************** */
// /* サブシェルカウントの検証 */

// void	test_SubshellCount(void)
// {
// 	t_list	*token_list;
// 	int		subshell_count;
// 	char	*input;

// 	input = "(ls && (echo hello) && (wc -l))";
// 	token_list = NULL;
// 	subshell_count = 0;
// 	// 入れ子になったサブシェルでカウントが正しく管理されるか
// 	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, lexer(input, &token_list));
// 	TEST_ASSERT_EQUAL_INT(OK, check_tokens_grammar(&token_list,
// 			&subshell_count));
// 	TEST_ASSERT_EQUAL_INT(0, subshell_count); // 最��的に0に戻っていることを確認
// 	ft_lstclear(&token_list, free_token);
// }

// void	test_SubshellCountUnbalanced(void)
// {
// 	t_list	*token_list;
// 	int		subshell_count;

// 	token_list = NULL;
// 	subshell_count = 0;
// 	// バランスの取れていないサブシェルはエラー
// 	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, lexer("(ls && (echo hello)",
// 			&token_list));
// 	TEST_ASSERT_EQUAL_INT(NG, check_tokens_grammar(&token_list,
// 			&subshell_count));
// 	TEST_ASSERT_NOT_EQUAL(0, subshell_count); // 0に戻っていないはず
// 	ft_lstclear(&token_list, free_token);
// }

//
/* ************************************************************************** */
// /* 新しいトークンを受け取れなくなる状況のテスト */

// void	test_ComplexErrorRecovery(void)
// {
// 	t_list	*token_list;
// 	int		subshell_count;
// 	char	*input;

// 	input = "(ls && echo hello || | grep pattern)";
// 	// 複雑���コマンドで文法エラーがあっても適切にクリーンアップされるか
// 	token_list = NULL;
// 	subshell_count = 0;
// 	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, lexer(input, &token_list));
// 	TEST_ASSERT_EQUAL_INT(NG, check_tokens_grammar(&token_list,
// 			&subshell_count));
// 	ft_lstclear(&token_list, free_token);
// }
