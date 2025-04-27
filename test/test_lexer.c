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

// Helper to check token values
static void	check_token(t_token *token, int expected_type,
		const char *expected_content)
{
	TEST_ASSERT_EQUAL_INT(expected_type, token->type);
	TEST_ASSERT_EQUAL_STRING(expected_content, token->content);
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
	t_token	*token1;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_NOT_NULL(tokens);
	token1 = (t_token *)tokens->content;
	check_token(token1, TERMINATOR, "\n");
}

void	test_SimpleCommand(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo hello", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_NOT_NULL(tokens);
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	check_token(token1, OPERAND_TEXT, "echo hello");
	check_token(token2, TERMINATOR, "\n");
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
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("ls | grep test", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "ls ");
	check_token(token2, OP_PIPE, "|");
	check_token(token3, OPERAND_TEXT, "grep test");
	check_token(token4, TERMINATOR, "\n");
	ft_lstclear(&tokens, free_token);
}

void	test_CommandWithRedirections(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	t_token	*token5;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("cat < infile > outfile", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(6, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	token5 = (t_token *)tokens->next->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "cat ");
	check_token(token2, OP_INPUT, "<");
	check_token(token3, OPERAND_TEXT, "infile ");
	check_token(token4, OP_OUTPUT, ">");
	check_token(token5, OPERAND_TEXT, "outfile");
	ft_lstclear(&tokens, free_token);
}

void	test_DoubleQuotes(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\"", &tokens);
	TEST_ASSERT_EQUAL_INT(5, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, QUOTE_DOUBLE, "\"");
	check_token(token3, OPERAND_TEXT, "hello world");
	check_token(token4, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_SingleQuotes(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo 'hello world'", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(5, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, QUOTE_SINGLE, "'");
	check_token(token3, OPERAND_TEXT, "hello world");
	check_token(token4, QUOTE_SINGLE, "'");
	ft_lstclear(&tokens, free_token);
}

void	test_Heredoc(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("cat << EOF", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	check_token(token1, OPERAND_TEXT, "cat ");
	check_token(token2, OP_HEREDOC, "<<");
	check_token(token3, OPERAND_TEXT, "EOF");
	ft_lstclear(&tokens, free_token);
}

void	test_Append(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo >> output.txt", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, OP_APPEND, ">>");
	check_token(token3, OPERAND_TEXT, "output.txt");
	ft_lstclear(&tokens, free_token);
}

void	test_AndOperator(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("command1 && command2", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	check_token(token1, OPERAND_TEXT, "command1 ");
	check_token(token2, OP_AND, "&&");
	check_token(token3, OPERAND_TEXT, "command2");
	ft_lstclear(&tokens, free_token);
}

void	test_OrOperator(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("command1 || command2", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	check_token(token1, OPERAND_TEXT, "command1 ");
	check_token(token2, OP_OR, "||");
	check_token(token3, OPERAND_TEXT, "command2");
	ft_lstclear(&tokens, free_token);
}

void	test_Parentheses(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("(command1)", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	check_token(token1, OP_OPEN, "(");
	check_token(token2, OPERAND_TEXT, "command1");
	check_token(token3, OP_CLOSE, ")");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommand(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("command1 | command2 && command3", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(6, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "command1 ");
	check_token(token2, OP_PIPE, "|");
	check_token(token3, OPERAND_TEXT, "command2 ");
	check_token(token4, OP_AND, "&&");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotes(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	t_token	*token5;
	t_token	*token6;
	t_token	*token7;
	t_token	*token8;
	t_token	*token9;
	t_token	*token10;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | grep 'test'", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(11, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	token5 = (t_token *)tokens->next->next->next->next->content;
	token6 = (t_token *)tokens->next->next->next->next->next->content;
	token7 = (t_token *)tokens->next->next->next->next->next->next->content;
	token8 = (t_token *)tokens->next->next->next->next->next->next->next->content;
	token9 = (t_token *)tokens->next->next->next->next->next->next->next->next->content;
	token10 = (t_token *)tokens->next->next->next->next->next->next->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, QUOTE_DOUBLE, "\"");
	check_token(token3, OPERAND_TEXT, "hello world");
	check_token(token4, QUOTE_DOUBLE, "\"");
	check_token(token5, OPERAND_TEXT, " ");
	check_token(token6, OP_PIPE, "|");
	check_token(token7, OPERAND_TEXT, "grep ");
	check_token(token8, QUOTE_SINGLE, "'");
	check_token(token9, OPERAND_TEXT, "test");
	check_token(token10, QUOTE_SINGLE, "'");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithRedirections(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("cat < input.txt > output.txt", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(6, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "cat ");
	check_token(token2, OP_INPUT, "<");
	check_token(token3, OPERAND_TEXT, "input.txt ");
	check_token(token4, OP_OUTPUT, ">");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithOperators(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	t_token	*token5;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo hello | grep test && ls -la", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(6, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	token5 = (t_token *)tokens->next->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo hello ");
	check_token(token2, OP_PIPE, "|");
	check_token(token3, OPERAND_TEXT, "grep test ");
	check_token(token4, OP_AND, "&&");
	check_token(token5, OPERAND_TEXT, "ls -la");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndOperators(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	t_token	*token5;
	t_token	*token6;
	t_token	*token7;
	t_token	*token8;
	t_token	*token9;
	t_token	*token10;
	t_token	*token11;
	t_token	*token12;
	t_token	*token13;
	t_token	*token_tail;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | grep 'test' && ls -la",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(14, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	token5 = (t_token *)tokens->next->next->next->next->content;
	token6 = (t_token *)tokens->next->next->next->next->next->content;
	token7 = (t_token *)tokens->next->next->next->next->next->next->content;
	token8 = (t_token *)tokens->next->next->next->next->next->next->next->content;
	token9 = (t_token *)tokens->next->next->next->next->next->next->next->next->content;
	token10 = (t_token *)tokens->next->next->next->next->next->next->next->next->next->content;
	token11 = (t_token *)tokens->next->next->next->next->next->next->next->next->next->next->content;
	token12 = (t_token *)tokens->next->next->next->next->next->next->next->next->next->next->next->content;
	token13 = (t_token *)tokens->next->next->next->next->next->next->next->next->next->next->next->next->content;
	token_tail = (t_token *)tokens->next->next->next->next->next->next->next->next->next->next->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, QUOTE_DOUBLE, "\"");
	check_token(token3, OPERAND_TEXT, "hello world");
	check_token(token4, QUOTE_DOUBLE, "\"");
	check_token(token5, OPERAND_TEXT, " ");
	check_token(token6, OP_PIPE, "|");
	check_token(token7, OPERAND_TEXT, "grep ");
	check_token(token8, QUOTE_SINGLE, "'");
	check_token(token9, OPERAND_TEXT, "test");
	check_token(token10, QUOTE_SINGLE, "'");
	check_token(token11, OPERAND_TEXT, " ");
	check_token(token12, OP_AND, "&&");
	check_token(token13, OPERAND_TEXT, "ls -la");
	check_token(token_tail, TERMINATOR, "\n");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithRedirectionsAndOperators(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("cat < input.txt > output.txt | grep test && ls -la",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(10, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "cat ");
	check_token(token2, OP_INPUT, "<");
	check_token(token3, OPERAND_TEXT, "input.txt ");
	check_token(token4, OP_OUTPUT, ">");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndRedirections(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" < input.txt > output.txt",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(10, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, QUOTE_DOUBLE, "\"");
	check_token(token3, OPERAND_TEXT, "hello world");
	check_token(token4, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndOperatorsAndRedirections(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | grep 'test' < input.txt > output.txt",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(16, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, QUOTE_DOUBLE, "\"");
	check_token(token3, OPERAND_TEXT, "hello world");
	check_token(token4, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndOperatorsAndRedirectionsAndParentheses(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | (grep 'test' < input.txt > output.txt)",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(18, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, QUOTE_DOUBLE, "\"");
	check_token(token3, OPERAND_TEXT, "hello world");
	check_token(token4, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndOperatorsAndRedirectionsAndParenthesesAndHeredoc(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | (grep 'test' << EOF < input.txt > output.txt)",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(20, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, QUOTE_DOUBLE, "\"");
	check_token(token3, OPERAND_TEXT, "hello world");
	check_token(token4, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_ComplexCommandWithQuotesAndOperatorsAndRedirectionsAndParenthesesAndHeredocAndAppend(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hello world\" | (grep 'test' << EOF < input.txt > output.txt >> append.txt)",
			&tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(22, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, QUOTE_DOUBLE, "\"");
	check_token(token3, OPERAND_TEXT, "hello world");
	check_token(token4, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_InvalidInput(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	t_token	*token3;
	t_token	*token4;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo | | grep", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(5, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	token3 = (t_token *)tokens->next->next->content;
	token4 = (t_token *)tokens->next->next->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, OP_PIPE, "|");
	check_token(token3, OP_PIPE, "|");
	check_token(token4, OPERAND_TEXT, "grep");
	ft_lstclear(&tokens, free_token);
}

void	test_foolish_duble_quote(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo \"hel(l>>>>>o' &&wo<<<<<<)r'ld", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, QUOTE_DOUBLE, "\"");
	ft_lstclear(&tokens, free_token);
}

void	test_foolish_single_quote(void)
{
	t_list	*tokens;
	t_token	*token1;
	t_token	*token2;
	int		exit_status;

	tokens = NULL;
	exit_status = lexer("echo 'hel(l>>>>>o\" &&w)o<<<<<<r\"ld", &tokens);
	TEST_ASSERT_EQUAL_INT(EXIT_S_SUCCESS, exit_status);
	TEST_ASSERT_EQUAL_INT(4, count_tokens(tokens));
	// Check each token's type and content
	token1 = (t_token *)tokens->content;
	token2 = (t_token *)tokens->next->content;
	check_token(token1, OPERAND_TEXT, "echo ");
	check_token(token2, QUOTE_SINGLE, "'");
	ft_lstclear(&tokens, free_token);
}
