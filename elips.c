#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CONTROLLER_DIR "app/controllers/"
#define MODEL_DIR "app/models/"

#define CONTROLLER_DATA "<?php\n\nclass $controller_name extends Controller {\n\n    public function index(){        \n        \n        \n        \n    }\n\n}\n"
#define MODEL_DATA "<?php\n\nclass $model_name extends Model {\n\n    static $table = 'table_name';\n\n    public static function init(){        \n        parent::setTable(self::$table);\n        parent::setFieldId('field_id_name');\n    }\n\n}\n"

char *str_replace(const char *substr, const char *replacement, const char *string){
    char *tok = NULL;
    char *newstr = NULL;
    char *oldstr = NULL;
    /* if either substr or replacement is NULL, duplicate string a let caller handle it */
    if ( substr == NULL || replacement == NULL ) return strdup (string);
    newstr = strdup (string);
    while ( (tok = strstr ( newstr, substr ))){
        oldstr = newstr;
        newstr = malloc ( strlen ( oldstr ) - strlen ( substr ) + strlen ( replacement ) + 1 );
        /*failed to alloc mem, free old string and return NULL */
        if ( newstr == NULL ){
          free (oldstr);
          return NULL;
        }
        memcpy ( newstr, oldstr, tok - oldstr );
        memcpy ( newstr + (tok - oldstr), replacement, strlen ( replacement ) );
        memcpy ( newstr + (tok - oldstr) + strlen( replacement ), tok + strlen ( substr ), strlen ( oldstr ) - strlen ( substr ) - ( tok - oldstr ) );
        memset ( newstr + strlen ( oldstr ) - strlen ( substr ) + strlen ( replacement ) , 0, 1 );
        free (oldstr);
    }
    return newstr;
}

int file_exist(char *filename){
    struct stat buffer;   
    return (stat (filename, &buffer) == 0);
}

int make_dir(char *dirname){
	struct stat st = {0};

	if (stat(dirname, &st) == -1) {
	    return mkdir(dirname, 0755);
	} else {
		printf("Directory exists!");
	}
	return 0;
}

int write_file(char *filename, char *str){
    FILE *fp;
	fp=fopen(filename, "w");
    fwrite(str, 1, strlen(str), fp);
    fclose(fp);
    return 1;
}

int generate_controller(char *controller_name){
    if(!file_exist(CONTROLLER_DIR)){
        printf("Directory \"%s\" not exists!\n", CONTROLLER_DIR);
        return 0;
    }

    char *controller_file = malloc(sizeof(CONTROLLER_DIR) + sizeof(controller_name) + 4);;
    strcat(controller_file, CONTROLLER_DIR);
    strcat(controller_file, controller_name);
    strcat(controller_file, ".php");
    
    if(file_exist(controller_file)){
        printf("Controller \"%s\" is exists!\n", controller_name);
        return 0;
    }

    char *controller_data = malloc(sizeof(CONTROLLER_DATA));
    strcpy(controller_data, CONTROLLER_DATA);
    controller_data = str_replace("$controller_name", controller_name, controller_data);

    return write_file(controller_file, controller_data);
}

int generate_model(char *model_name){
    if(!file_exist(MODEL_DIR)){
        printf("Directory \"%s\" not exists!\n", MODEL_DIR);
        return 0;
    }

    char *model_file = malloc(sizeof(MODEL_DIR) + sizeof(model_name) + 4);
    strcat(model_file, MODEL_DIR);
    strcat(model_file, model_name);
    strcat(model_file, ".php");

    if(file_exist(model_file)){
        printf("Model \"%s\" is exists!\n", model_name);
        return 0;
    }
    
    char *model_data = malloc(sizeof(MODEL_DATA));
    strcpy(model_data, MODEL_DATA);
    strcpy(model_data, str_replace("$model_name", model_name, model_data));
    
    return write_file(model_file, model_data);
}

int main(int argc, char *argv[]){
	if(argc > 2){
		if (strcmp(argv[1], "generate") == 0 || strcmp(argv[1], "-g") == 0){
            if (strcmp(argv[2], "controller") == 0){
                if(strcmp(argv[3], "") != 0){
                    char *controller_name = malloc(sizeof(argv[3]));
                    strcpy(controller_name, argv[3]);
                    if(generate_controller(controller_name)){
	    				printf("Success generate controler \"%s\"\n", argv[3]);
	    			} else {
	    				printf("Failed generate controler \"%s\"!\n", argv[3]);
	    			}
	    		} else {
                    printf("Undefined controller name!\n");
                }
	    	}
            else if (strcmp(argv[2], "model") == 0){
                if(strcmp(argv[3], "") != 0){
                    char *model_name = malloc(sizeof(argv[3]));
                    strcpy(model_name, argv[3]);
                    if(generate_model(model_name)){
                        printf("Success generate model \"%s\"\n", argv[3]);
                    } else {
                        printf("Failed generate model \"%s\"!\n", argv[3]);
                    }
                } else {
                    printf("Undefined model name!\n");
                }
            }
    	}	
	}

	return 0;
}