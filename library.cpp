namespace test {
int test_function() { return 0; }
inline namespace v1 {
	int test_function() { return 1; }
}
}
