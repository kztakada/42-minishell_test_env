#include "lexer.h"
#include "unity.h"

// using source file
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
TEST_SOURCE_FILE("dictionary.c")
TEST_SOURCE_FILE("lexicize_func_sign.c")
TEST_SOURCE_FILE("lexicize_text.c")
TEST_SOURCE_FILE("lexing__utils.c")

// Helper function to count tokens
static int	count_tokens(t_list *token_list)
{
	int	count;

	count = 0;
	while (token_list)
	{
		count++;
		token_list = token_list->next;
	}
	return (count);
}

t_list			*tester_tokens;

// Helper to check token values
static t_token	*make_token(int token_index, t_list *tokens)
{
	tester_tokens = tokens;
	while (token_index > 0)
	{
		tester_tokens = tester_tokens->next;
		token_index--;
	}
	return ((t_token *)tester_tokens->content);
}

static void	check_token(t_token *token, int expected_id, int expected_type,
		const char *expected_content)
{
	TEST_ASSERT_EQUAL_UINT(expected_id, token->id);
	TEST_ASSERT_EQUAL_INT(expected_type, token->type);
	TEST_ASSERT_EQUAL_STRING(expected_content, token->value);
}

void	setUp(void)
{
	// Setup code runs before each test
}

void	tearDown(void)
{
	// Cleanup code runs after each test
}

void	test_NULLInput(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer(NULL, &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_FAILURE, exit_status);
	TEST_ASSERT_NULL(tokens);
}

void	test_EmptyInput(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_NOT_NULL(tokens);
	check_token(make_token(0, tokens), 0, TERMINATOR, "\n");
}

void	test_SimpleCommand(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo hello", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_NOT_NULL(tokens);
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo hello");
	check_token(make_token(1, tokens), 1, TERMINATOR, "\n");
	ft_lstclear(&tokens, free_token);
}

void	test_MultipleWords(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("ls -la documents", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(2, count_tokens(tokens));
	// Additional assertions for token content would go here
	ft_lstclear(&tokens, free_token);
}

void	test_CommandWithPipe(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("ls | grep test", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "ls ");
	check_token(make_token(1, tokens), 1, OP_PIPE, "|");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "grep test");
	check_token(make_token(3, tokens), 3, TERMINATOR, "\n");
	ft_lstclear(&tokens, free_token);
}

void	test_CommandWithRedirections(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("cat < infile > outfile", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(6, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "cat ");
	check_token(make_token(1, tokens), 1, OP_INPUT, "<");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "infile ");
	check_token(make_token(3, tokens), 3, OP_OUTPUT, ">");
	check_token(make_token(4, tokens), 4, OPERAND_TEXT, "outfile");
	ft_lstclear(&tokens, free_token);
}

void	test_DoubleQuotes(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\"", &tokens);
	TEST_ASSERT_EQUAL_INT(5, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, QUOTE_DOUBLE, "\"");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "hello world");
	check_token(make_token(3, tokens), 3, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_SingleQuotes(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo 'hello world'", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(5, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, QUOTE_SINGLE, "'");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "hello world");
	check_token(make_token(3, tokens), 3, QUOTE_SINGLE, "'");
	ft_lstclear(&tokens, free_token);
}

void	test_Heredoc(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("cat << EOF", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "cat ");
	check_token(make_token(1, tokens), 1, OP_HEREDOC, "<<");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "EOF");
	ft_lstclear(&tokens, free_token);
}

void	test_Append(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo >> output.txt", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, OP_APPEND, ">>");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "output.txt");
	ft_lstclear(&tokens, free_token);
}

void	test_AndOperator(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("command1 && command2", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "command1 ");
	check_token(make_token(1, tokens), 1, OP_AND, "&&");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "command2");
	ft_lstclear(&tokens, free_token);
}

void	test_OrOperator(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("command1 || command2", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "command1 ");
	check_token(make_token(1, tokens), 1, OP_OR, "||");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "command2");
	ft_lstclear(&tokens, free_token);
}

void	test_Parentheses(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("(command1)", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OP_OPEN, "(");
	check_token(make_token(1, tokens), 1, OPERAND_TEXT, "command1");
	check_token(make_token(2, tokens), 2, OP_CLOSE, ")");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommand(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("command1 | command2 && command3", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(6, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "command1 ");
	check_token(make_token(1, tokens), 1, OP_PIPE, "|");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "command2 ");
	check_token(make_token(3, tokens), 3, OP_AND, "&&");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotes(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | grep 'test'", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(11, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, QUOTE_DOUBLE, "\"");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "hello world");
	check_token(make_token(3, tokens), 3, QUOTE_DOUBLE, "\"");
	check_token(make_token(4, tokens), 4, OPERAND_TEXT, " ");
	check_token(make_token(5, tokens), 5, OP_PIPE, "|");
	check_token(make_token(6, tokens), 6, OPERAND_TEXT, "grep ");
	check_token(make_token(7, tokens), 7, QUOTE_SINGLE, "'");
	check_token(make_token(8, tokens), 8, OPERAND_TEXT, "test");
	check_token(make_token(9, tokens), 9, QUOTE_SINGLE, "'");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithRedirections(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("cat < input.txt > output.txt", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(6, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "cat ");
	check_token(make_token(1, tokens), 1, OP_INPUT, "<");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "input.txt ");
	check_token(make_token(3, tokens), 3, OP_OUTPUT, ">");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithOperators(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo hello | grep test && ls -la", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(6, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo hello ");
	check_token(make_token(1, tokens), 1, OP_PIPE, "|");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "grep test ");
	check_token(make_token(3, tokens), 3, OP_AND, "&&");
	check_token(make_token(4, tokens), 4, OPERAND_TEXT, "ls -la");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndOperators(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | grep 'test' && ls -la",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(14, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, QUOTE_DOUBLE, "\"");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "hello world");
	check_token(make_token(3, tokens), 3, QUOTE_DOUBLE, "\"");
	check_token(make_token(4, tokens), 4, OPERAND_TEXT, " ");
	check_token(make_token(5, tokens), 5, OP_PIPE, "|");
	check_token(make_token(6, tokens), 6, OPERAND_TEXT, "grep ");
	check_token(make_token(7, tokens), 7, QUOTE_SINGLE, "'");
	check_token(make_token(8, tokens), 8, OPERAND_TEXT, "test");
	check_token(make_token(9, tokens), 9, QUOTE_SINGLE, "'");
	check_token(make_token(10, tokens), 10, OPERAND_TEXT, " ");
	check_token(make_token(11, tokens), 11, OP_AND, "&&");
	check_token(make_token(12, tokens), 12, OPERAND_TEXT, "ls -la");
	check_token(make_token(13, tokens), 13, TERMINATOR, "\n");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithRedirectionsAndOperators(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("cat < input.txt > output.txt | grep test && ls -la",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(10, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "cat ");
	check_token(make_token(1, tokens), 1, OP_INPUT, "<");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "input.txt ");
	check_token(make_token(3, tokens), 3, OP_OUTPUT, ">");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndRedirections(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" < input.txt > output.txt",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(10, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, QUOTE_DOUBLE, "\"");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "hello world");
	check_token(make_token(3, tokens), 3, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndOperatorsAndRedirections(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | grep 'test' < input.txt > output.txt",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(16, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, QUOTE_DOUBLE, "\"");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "hello world");
	check_token(make_token(3, tokens), 3, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndOperatorsAndRedirectionsAndParentheses(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | (grep 'test' < input.txt > output.txt)",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(18, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, QUOTE_DOUBLE, "\"");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "hello world");
	check_token(make_token(3, tokens), 3, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndOperatorsAndRedirectionsAndParenthesesAndHeredoc(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | (grep 'test' << EOF < input.txt > output.txt)",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(20, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, QUOTE_DOUBLE, "\"");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "hello world");
	check_token(make_token(3, tokens), 3, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndOperatorsAndRedirectionsAndParenthesesAndHeredocAndAppend(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | (grep 'test' << EOF < input.txt > output.txt >> append.txt)",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(22, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, QUOTE_DOUBLE, "\"");
	check_token(make_token(2, tokens), 2, OPERAND_TEXT, "hello world");
	check_token(make_token(3, tokens), 3, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_InvalidInput(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo | | grep", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(5, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, OP_PIPE, "|");
	check_token(make_token(2, tokens), 2, OP_PIPE, "|");
	check_token(make_token(3, tokens), 3, OPERAND_TEXT, "grep");
	ft_lstclear(&tokens, free_token);
}

void	test_foolish_duble_quote(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hel(l>>>>>o' &&wo<<<<<<)r'ld", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_foolish_single_quote(void)
{
	t_list	*tokens;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo 'hel(l>>>>>o\" &&w)o<<<<<<r\"ld", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OPERAND_TEXT, "echo ");
	check_token(make_token(1, tokens), 1, QUOTE_SINGLE, "'");
	ft_lstclear(&tokens, free_token);
}

void	test_foolish_single_quote_02(void)
{
	t_list	*tokens;
	int		exit_status;
	char	*test3;

	test3 = " << >> && || | < > ( ) ";
	tokens = NULL;
	exit_status = lexer(")h' << >> && || | < > ( ) ' \"", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(8, count_tokens(tokens));
	// Check each token's type and content
	check_token(make_token(0, tokens), 0, OP_CLOSE, ")");
	check_token(make_token(1, tokens), 1, OPERAND_TEXT, "h");
	check_token(make_token(2, tokens), 2, QUOTE_SINGLE, "'");
	check_token(make_token(3, tokens), 3, OPERAND_TEXT, test3);
	check_token(make_token(7, tokens), 7, TERMINATOR, "\n");
	ft_lstclear(&tokens, free_token);
}
