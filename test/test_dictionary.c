#include "dictionary.h"
#include "unity.h"
#include <string.h>

// For mock implementation
// extern int	ft_strncmp(const char *s1, const char *s2, size_t n);

// using source file
TEST_SOURCE_FILE("ft_strncmp.c")

// Define constants used in the implementation

void	setUp(void)
{
	// This is run before EACH test
}

void	tearDown(void)
{
	// This is run after EACH test
}

void	test_lookup_dict_consecutive_operators(void)
{
	char		*input;
	char		*original;
	t_dict_out	d_out;

	// Test with consecutive operators (e.g. "<<>>")
	input = "<<>>test";
	original = input;
	d_out = lookup_dict(input, OPERATORS_DICT);
	TEST_ASSERT_EQUAL_INT(TRUE, d_out.in_d);
	// d_out.in_d should be TRUE
	TEST_ASSERT_EQUAL_INT(0, d_out.d_index);
	// Should match "<<" first
	TEST_ASSERT_EQUAL_INT(2, d_out.nx_str - original);
	// Pointer should move by 2
	TEST_ASSERT_EQUAL_STRING(">>test", d_out.nx_str);
	TEST_ASSERT_NULL(d_out.error); // d_out.error should be NULL
}

void	test_lookup_dict_multiple_potential_matches(void)
{
	t_dict_out	d_out;

	// Test with string that could match multiple operators but should match longest first
	const char *input = ">>>test"; // Should match ">>" not ">"
	d_out = lookup_dict(input, OPERATORS_DICT);
	TEST_ASSERT_EQUAL_INT(TRUE, d_out.in_d);        // d_out.in_d should be TRUE
	TEST_ASSERT_EQUAL_INT(1, d_out.d_index);        // Should match ">>"
	TEST_ASSERT_EQUAL_INT(2, d_out.nx_str - input); // Pointer should move by 2
	TEST_ASSERT_EQUAL_STRING(">test", d_out.nx_str);
	TEST_ASSERT_NULL(d_out.error); // d_out.error should be NULL
}

void	test_lookup_dict_operators_with_spaces(void)
{
	char		*input;
	t_dict_out	d_out;

	// Make sure operators with spaces before/after are handled correctly
	input = "| command";
	d_out = lookup_dict(input, OPERATORS_DICT);
	TEST_ASSERT_EQUAL_INT(TRUE, d_out.in_d); // d_out.in_d should be TRUE
	TEST_ASSERT_EQUAL_INT(4, d_out.d_index); // Should match "|"
	TEST_ASSERT_EQUAL_INT(1, d_out.nx_str - input);
	TEST_ASSERT_EQUAL_STRING(" command", d_out.nx_str);
	TEST_ASSERT_NULL(d_out.error); // d_out.error should be NULL
}

void	test_lookup_dict_case_sensitivity(void)
{
	char		*input;
	t_dict_out	d_out;

	// Test case sensitivity - ">" is different from ">"
	input = "< >test";
	d_out = lookup_dict(input, OPERATORS_DICT);
	TEST_ASSERT_EQUAL_INT(TRUE, d_out.in_d); // d_out.in_d should be TRUE
	TEST_ASSERT_EQUAL_INT(5, d_out.d_index); // Should match "<"
	TEST_ASSERT_EQUAL_INT(1, d_out.nx_str - input);
	TEST_ASSERT_EQUAL_STRING(" >test", d_out.nx_str);
	TEST_ASSERT_NULL(d_out.error); // d_out.error should be NULL
}

void	test_lookup_dict_with_null_subject(void)
{
	char		*input;
	t_dict_out	d_out;

	// Test with NULL subject - this would test error handling
	input = NULL;
	d_out = lookup_dict(input, OPERATORS_DICT);
	TEST_ASSERT_EQUAL_INT(FALSE, d_out.in_d); // d_out.in_d should be FALSE
	TEST_ASSERT_EQUAL_INT(-1, d_out.d_index);
	TEST_ASSERT_NULL(d_out.nx_str);
	// d_out.nx_str should be NULL
	TEST_ASSERT_EQUAL_STRING(INVALID_SUBJECT, d_out.error);
	// d_out.error should be INVALID_SUBJECT
}

void	test_lookup_dict_with_null_dict(void)
{
	char		*input;
	t_dict_out	d_out;

	// Test with NULL dictionary
	input = "test";
	d_out = lookup_dict(input, NULL);
	TEST_ASSERT_EQUAL_INT(FALSE, d_out.in_d); // d_out.in_d should be FALSE
	TEST_ASSERT_EQUAL_INT(-1, d_out.d_index);
	TEST_ASSERT_NULL(d_out.nx_str);
	// d_out.nx_str should be NULL
	TEST_ASSERT_EQUAL_STRING(INVALID_DICT, d_out.error);
	// d_out.error should be INVALID_DICT
}

void	test_lookup_dict_boundary_conditions(void)
{
	char		*input;
	char		*original;
	t_dict_out	d_out;

	// Test boundary conditions - operators at the end of string
	original = "echo |";
	input = original + 5; // Skip "echo "
	d_out = lookup_dict(input, OPERATORS_DICT);
	TEST_ASSERT_EQUAL_INT(TRUE, d_out.in_d); // d_out.in_d should be TRUE
	TEST_ASSERT_EQUAL_INT(4, d_out.d_index); // Should match "|"
	TEST_ASSERT_EQUAL_INT(1, d_out.nx_str - input);
	TEST_ASSERT_EQUAL_STRING("", d_out.nx_str);
	TEST_ASSERT_NULL(d_out.error); // d_out.error should be NULL
}

void	test_lookup_dict_similar_operators(void)
{
	char		*input;
	t_dict_out	d_out;

	// Test with operators that start similarly (e.g. "<" vs "<<")
	input = "<test";
	d_out = lookup_dict(input, OPERATORS_DICT);
	TEST_ASSERT_EQUAL_INT(TRUE, d_out.in_d); // d_out.in_d should be TRUE
	TEST_ASSERT_EQUAL_INT(5, d_out.d_index); // Should match "<", not "<<"
	TEST_ASSERT_EQUAL_INT(1, d_out.nx_str - input);
	TEST_ASSERT_EQUAL_STRING("test", d_out.nx_str);
	TEST_ASSERT_NULL(d_out.error); // d_out.error should be NULL
}

void	test_lookup_dict_custom_dictionary(void)
{
	char		*input;
	t_dict_out	d_out;

	// Test with a custom dictionary
	input = "FOObar";
	d_out = lookup_dict(input, " FOO BAR BAZ");
	TEST_ASSERT_EQUAL_INT(TRUE, d_out.in_d); // d_out.in_d should be TRUE
	TEST_ASSERT_EQUAL_INT(0, d_out.d_index); // Should match "FOO"
	TEST_ASSERT_EQUAL_INT(3, d_out.nx_str - input);
	TEST_ASSERT_EQUAL_STRING("bar", d_out.nx_str);
	TEST_ASSERT_NULL(d_out.error); // d_out.error should be NULL
}

void	test_lookup_dict_empty_string(void)
{
	char		*input;
	t_dict_out	d_out;

	// Test with an empty string
	input = "";
	d_out = lookup_dict(input, OPERATORS_DICT);
	TEST_ASSERT_EQUAL_INT(FALSE, d_out.in_d); // d_out.in_d should be FALSE
	TEST_ASSERT_EQUAL_INT(-1, d_out.d_index);
	TEST_ASSERT_NULL(d_out.nx_str);
	// d_out.nx_str should be NULL
	TEST_ASSERT_EQUAL_STRING(NO_MATCH_DICT, d_out.error);
	// d_out.error should be NO_MATCH_DICT
}

void	test_lookup_dict_empty_dictionary(void)
{
	char		*input;
	t_dict_out	d_out;

	// Test with an empty dictionary
	input = "test";
	d_out = lookup_dict(input, "");
	TEST_ASSERT_EQUAL_INT(FALSE, d_out.in_d); // d_out.in_d should be FALSE
	TEST_ASSERT_EQUAL_INT(-1, d_out.d_index);
	TEST_ASSERT_NULL(d_out.nx_str);
	// d_out.nx_str should be NULL
	TEST_ASSERT_EQUAL_STRING(INVALID_DICT, d_out.error);
	// d_out.error should be INVALID_DICT
}

void	test_lookup_dict_invalid_dictionary(void)
{
	char		*input;
	t_dict_out	d_out;

	// Test with an invalid dictionary (e.g. no separators)
	input = "test";
	d_out = lookup_dict(input, "ttest");
	TEST_ASSERT_EQUAL_INT(FALSE, d_out.in_d); // d_out.in_d should be FALSE
	TEST_ASSERT_EQUAL_INT(-1, d_out.d_index);
	TEST_ASSERT_NULL(d_out.nx_str);
	// d_out.nx_str should be NULL
	TEST_ASSERT_EQUAL_STRING(INVALID_DICT, d_out.error);
	// d_out.error should be NO_MATCH_DICT
}
