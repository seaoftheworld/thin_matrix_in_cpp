#include <stdio.h>

int test_00_basic_entity_renderer();
int test_01_specular_entity_renderer();
int test_02_multi_lights_entity_renderer();
int test_03_terrain();
int test_04();

int main() {
    
    int result = -1;

    printf("\n\n\n\n");
    printf("test starts\n");

    bool run = true;
    while (run) {

        int input_value = -1;

        printf("    please input a number to run the corresponding test:\n");
        printf("    input \'999\' to quit.\n");
        scanf("%d", &input_value);

        printf("    %d is input\n", input_value);

        switch (input_value) {
            case 0:
            // printf("    test-%d not supported yet.\n\n", input_value);
                result = test_00_basic_entity_renderer();
            break;

            case 1:
                result = test_01_specular_entity_renderer();
                // run = false;
            break;

            case 2:
                result = test_02_multi_lights_entity_renderer();
                // run = false;
            break;

            case 3:
                result = test_03_terrain();
                run = false;
            break;

            case 4:
                result = test_04();
                run = false;
            break;


            case 999:
                run = false;
            break;

            default:
            printf("    test-%d not supported yet.\n\n", input_value);
            break;
        }
    }

    return result;
}
