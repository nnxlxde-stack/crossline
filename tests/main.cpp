import crossline_ui_reactive_tests;
import light_test;

int main() {
    register_crossline_ui_reactive_tests();
    return light_test::run_all_tests_and_print();
}