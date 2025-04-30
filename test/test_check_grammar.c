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
TEST_SOURCE_FILE("grammar_redirect.c")
TEST_SOURCE_FILE("grammar_subshell.c")
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

// 基本的なテスト関数 - 文法チェックが成功するかどうかをテスト
static void	test_grammar_validation(const char *input, int expected_result)
{
	t_list	*token_list;
	int		subshell_count;
	int		result;

	token_list = NULL;
	subshell_count = 0;
	// lexerでトークンリストを取得
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, lexer((char *)input, &token_list));
	// 文法チェックを実行
	result = check_tokens_grammar(&token_list, &subshell_count);
	// 結果を検証
	TEST_ASSERT_EQUAL_INT(expected_result, result);
	// クリーンアップ
	ft_lstclear(&token_list, free_token);
}

// エラー時のトークンポインタを検証するヘルパー関数
static void	test_grammar_error_token(const char *input, int expected_id,
		t_token_type expected_type, const char *expected_value)
{
	t_list	*token_list;
	int		subshell_count;
	int		result;
	t_token	*error_token;
	char	*err_text;

	// printf("line_num: %d\n", line_num);
	// char	*line_num_str;
	token_list = NULL;
	subshell_count = 0;
	// lexerでトークンリストを取得
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, lexer((char *)input, &token_list));
	// 文法チェックを実行
	result = check_tokens_grammar(&token_list, &subshell_count);
	// エラーを期待
	TEST_ASSERT_EQUAL_INT(NG, result);
	// エラートークンの検証
	TEST_ASSERT_NOT_NULL(token_list);
	error_token = (t_token *)(token_list->content);
	if (error_token->type == TERMINATOR)
		err_text = "newline";
	else if (error_token->type == OPERAND_TEXT)
		err_text = replase_first_ifs_with_null(error_token->value);
	else
		err_text = error_token->value;
	TEST_ASSERT_NOT_NULL(error_token);
	TEST_ASSERT_EQUAL_UINT(expected_id, error_token->id);
	TEST_ASSERT_EQUAL_INT(expected_type, error_token->type);
	TEST_ASSERT_EQUAL_STRING(expected_value, err_text);
	// クリーンアップ
	ft_lstclear(&token_list, free_token);
}

/* 基本的なコマンドのテスト */

void	test_SimpleCommand(void)
{
	test_grammar_validation("echo hello", OK);
}

void	test_CommandWithArguments(void)
{
	test_grammar_validation("ls -la", OK);
}

/* パイプ演算子のテスト */

void	test_SimplePipe(void)
{
	test_grammar_validation("echo hello | grep h", OK);
}

void	test_MultiPipe(void)
{
	test_grammar_validation("ls | grep a | wc -l", OK);
}

void	test_PipeWithError(void)
{
	// パイプの後にコマンドがない場合はエラー
	test_grammar_error_token("ls |", 2, TERMINATOR, "newline");
}

void	test_PipeStartError(void)
{
	// パイプから始まる場合はエラー
	test_grammar_error_token("| ls", 0, OP_PIPE, "|");
}

void	test_DoublePipeError(void)
{
	// 連続したパイプはエラー
	test_grammar_error_token("ls | | grep a", 2, OP_PIPE, "|");
}

/* リダイレクト演算子のテスト */

void	test_RedirectInput(void)
{
	test_grammar_validation("cat < input.txt", OK);
}

void	test_RedirectOutput(void)
{
	test_grammar_validation("ls > output.txt", OK);
}

void	test_RedirectAppend(void)
{
	test_grammar_validation("echo hello >> log.txt", OK);
}

void	test_RedirectHeredoc(void)
{
	test_grammar_validation("cat << EOF", OK);
}

void	test_RedirectCombined(void)
{
	test_grammar_validation("cat < input.txt > output.txt", OK);
}

void	test_RedirectError(void)
{
	// リダイレクトの後にファイル名がない場合はエラー
	test_grammar_error_token("ls >", 2, TERMINATOR, "newline");
}

void	test_RedirectStart(void)
{
	test_grammar_error_token("> output.txt", 0, OP_OUTPUT, ">");
}

/* 論理演算子のテスト */

void	test_AndOperator(void)
{
	test_grammar_validation("ls && echo success", OK);
}

void	test_OrOperator(void)
{
	test_grammar_validation("ls || echo failure", OK);
}

void	test_AndOrCombined(void)
{
	test_grammar_validation("ls && echo success || echo failure", OK);
}

void	test_AndError(void)
{
	// ANDの後にコマンドがない場合はエラー
	test_grammar_error_token("ls &&", 2, TERMINATOR, "newline");
}

void	test_OrError(void)
{
	// ORの後にコマンドがない場合はエラー
	test_grammar_error_token("ls ||", 2, TERMINATOR, "newline");
}

void	test_AndStartError(void)
{
	// ANDから始まる場合はエラー
	test_grammar_error_token("&& ls", 0, OP_AND, "&&");
}

void	test_OrStartError(void)
{
	// ORから始まる場合はエラー
	test_grammar_error_token("|| ls", 0, OP_OR, "||");
}

/* サブシェルのテスト */

void	test_SimpleSubshell(void)
{
	test_grammar_validation("(ls)", OK);
}

void	test_NestedSubshell(void)
{
	test_grammar_validation("(ls && (echo hello))", OK);
}

void	test_SubshellWithPipe(void)
{
	test_grammar_validation("(ls) | grep a", OK);
}

void	test_SubshellUnbalancedOpen(void)
{
	// 開きカッコだけの場合はエラー
	test_grammar_error_token("(ls", 2, TERMINATOR, "newline");
}

void	test_SubshellUnbalancedClose(void)
{
	// 閉じカッコだけの場合はエラー
	test_grammar_error_token("ls)", 1, OP_CLOSE, ")");
}

void	test_SubshellEmpty(void)
{
	// 空のサブシェルはエラー
	test_grammar_error_token("()", 0, OP_CLOSE, ")");
}

/* クォートのテスト */

void	test_SingleQuotes(void)
{
	test_grammar_validation("echo 'hello world'", OK);
}

void	test_DoubleQuotes(void)
{
	test_grammar_validation("echo \"hello world\"", OK);
}

void	test_MixedQuotes(void)
{
	test_grammar_validation("echo \"hello 'world'\"", OK);
}

/* 複合演算子のテスト */

void	test_ComplexCommand1(void)
{
	test_grammar_validation("ls -la | grep a > output.txt", OK);
}

void	test_ComplexCommand2(void)
{
	test_grammar_validation("(cat < input.txt) | grep a && echo success", OK);
}

void	test_ComplexCommand3(void)
{
	test_grammar_validation("ls && (echo success | grep s) || echo failure",
		OK);
}

void	test_ComplexCommand4(void)
{
	test_grammar_validation("(ls && echo success) || (echo failure && ls -la)",
		OK);
}

void	test_ComplexCommand5(void)
{
	test_grammar_validation("cat << EOF | grep pattern > output.txt", OK);
}

/* 演算子の組み合わせの網羅的なテスト */

void	test_AllOperatorsCombination1(void)
{
	char	*input;

	input = "ls -la | grep a > file && echo 'success' || echo \"failure\"";
	test_grammar_validation(input, OK);
}

void	test_AllOperatorsCombination2(void)
{
	char	*input;

	input = "(cat < input.txt | grep pattern) > output.txt && echo done";
	test_grammar_validation(input, OK);
}

void	test_AllOperatorsCombination3(void)
{
	test_grammar_validation("cat << EOF | (grep pattern && wc -l) >> log.txt",
		OK);
}

void	test_AllOperatorsCombination4(void)
{
	char	*input;

	input = "(ls -la > file) && (cat file | grep pattern)|| echo 'not found'";
	test_grammar_validation(input, OK);
}

void	test_AllOperatorsCombination5(void)
{
	test_grammar_validation("echo \"start\" && (cat < file1 > file2) | wc -l",
		OK);
}

/* エラー��ースの網羅的な�����スト */

void	test_ErrorCombination1(void)
{
	test_grammar_error_token("ls | && cat", 2, OP_AND, "&&");
	// パイプと論理演算子が隣接
}

void	test_ErrorCombination2(void)
{
	test_grammar_error_token("ls > > file", 2, OP_OUTPUT, ">");
	// 連続したリダイレクト
}

void	test_ErrorCombination3(void)
{
	test_grammar_error_token("(ls &&) || echo", 2, OP_CLOSE, ")");
	// 不完全なサブシェル内の式
}

void	test_ErrorCombination4(void)
{
	test_grammar_error_token("ls (cat file)", 1, OP_OPEN, "(");
	// 無効なサブシェルの使用
}

void	test_Combination5(void)
{
	test_grammar_error_token("ls | > file", 2, OP_OUTPUT, ">");
	// パイプの後に直接リ��イレクト
}

/* エラートークンの検証テスト */

void	test_ErrorToken_AndOr(void)
{
	// "&&" と "||" のみの入力では最初の "&&" がエラートークンとして返される
	test_grammar_error_token("&& ||", 0, OP_AND, "&&");
}

void	test_ErrorToken_OrAfterCommand(void)
{
	// "echo aaa && ||" の場合は "||" がエラートークンとして返される
	test_grammar_error_token("echo aaa && ||", 2, OP_OR, "||");
}

void	test_ErrorToken_PipeLine(void)
{
	// パイプの後にコマンドがない場合、パイプがエラートークン
	test_grammar_error_token("ls |", 2, TERMINATOR, "newline");
}

void	test_ErrorToken_StartWithPipe(void)
{
	// パイプから始まる場合、パイプがエラートークン
	test_grammar_error_token("| ls", 0, OP_PIPE, "|");
}

void	test_ErrorToken_DoublePipe(void)
{
	// 連続したパイプの場合、2つ目のパイプがエラートークン
	test_grammar_error_token("ls | | grep", 2, OP_PIPE, "|");
}

void	test_ErrorToken_RedirectWithoutFile(void)
{
	// リダイレクト後にファイル名がない場合、リダイレクトがエラートークン
	test_grammar_error_token("ls >", 2, TERMINATOR, "newline");
}

void	test_ErrorToken_AndWithoutCommand(void)
{
	// AND演算子の後にコマンドがない場合、ANDがエラートークン
	test_grammar_error_token("ls &&", 2, TERMINATOR, "newline");
}

void	test_ErrorToken_OrWithoutCommand(void)
{
	// OR演算子の後にコマンドがない場合、ORがエラートークン
	test_grammar_error_token("ls ||", 2, TERMINATOR, "newline");
}

void	test_ErrorToken_StartWithAnd(void)
{
	// AND演算子から始まる場合、ANDがエラートークン
	test_grammar_error_token("&& ls", 0, OP_AND, "&&");
}

void	test_ErrorToken_StartWithOr(void)
{
	// OR演算子から始まる場合、ORがエラートークン
	test_grammar_error_token("|| ls", 0, OP_OR, "||");
}

void	test_ErrorToken_UnbalancedOpenParen(void)
{
	// 開きカッコのみの場合、終端のトークンがエラートークン（閉じカッコがない）
	test_grammar_error_token("(ls", 2, TERMINATOR, "newline");
}

void	test_ErrorToken_UnbalancedCloseParen(void)
{
	// 閉じカッコが余分にある場合、閉じカッコがエラートークン
	test_grammar_error_token("ls)", 1, OP_CLOSE, ")");
}

void	test_ErrorToken_ComplexErrors(void)
{
	// 複雑なコマンドでのエラーケース
	test_grammar_error_token("ls | && cat", 2, OP_AND, "&&");
	test_grammar_error_token("ls > > file", 2, OP_OUTPUT, ">");
	test_grammar_error_token("(ls &&) || echo", 2, OP_CLOSE, ")");
	test_grammar_error_token("ls (cat file)", 1, OP_OPEN, "(");
}

/* サブシェル内部のエラーテスト */

void	test_ErrorToken_InSubshell(void)
{
	// サブシェル内のエラー
	test_grammar_error_token("(ls | | wc)", 3, OP_PIPE, "|");
	test_grammar_error_token("(echo && || cat)", 3, OP_OR, "||");
	test_grammar_error_token("(echo > )", 2, OP_CLOSE, ")");
}

/* 複合的なエラーケース */

void	test_ErrorToken_NestedSubshellErrors(void)
{
	// 入れ子になったサブシェル内のエラー
	test_grammar_error_token("(ls && (echo | | cat))", 6, OP_PIPE, "|");
	test_grammar_error_token("(ls && (echo &&) || cat)", 5, OP_CLOSE, ")");
}

/* サブシェルカウントの検証 */

void	test_SubshellCount(void)
{
	t_list	*token_list;
	int		subshell_count;
	char	*input;

	input = "(ls && (echo hello) && (wc -l))";
	token_list = NULL;
	subshell_count = 0;
	// 入れ子になったサブシェルでカウントが正しく管理されるか
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, lexer(input, &token_list));
	TEST_ASSERT_EQUAL_INT(OK, check_tokens_grammar(&token_list,
			&subshell_count));
	TEST_ASSERT_EQUAL_INT(0, subshell_count); // 最終的に0に戻っていることを確認
	ft_lstclear(&token_list, free_token);
}

void	test_SubshellCountUnbalanced(void)
{
	t_list	*token_list;
	int		subshell_count;

	token_list = NULL;
	subshell_count = 0;
	// バランスの取れていないサブシェルはエラー
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, lexer("(ls && (echo hello)",
			&token_list));
	TEST_ASSERT_EQUAL_INT(NG, check_tokens_grammar(&token_list,
			&subshell_count));
	TEST_ASSERT_NOT_EQUAL(0, subshell_count); // 0に戻っていないはず
	ft_lstclear(&token_list, free_token);
}

/* 新しいトークンを受け取れなくなる状況のテスト */

void	test_ComplexErrorRecovery(void)
{
	t_list	*token_list;
	int		subshell_count;
	char	*input;

	input = "(ls && echo hello || | grep pattern)";
	// 複雑なコマンドで文法エラーがあっても適切にクリーンアップされるか
	token_list = NULL;
	subshell_count = 0;
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, lexer(input, &token_list));
	TEST_ASSERT_EQUAL_INT(NG, check_tokens_grammar(&token_list,
			&subshell_count));
	ft_lstclear(&token_list, free_token);
}
