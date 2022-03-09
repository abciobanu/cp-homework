/* Ciobanu Andrei-Bogdan 312CB */

#include "utils.h"

#include <stdlib.h>
#include <string.h>

#define OUTPUT_FILENAME_SIZE 255

/* returneaza dimensiunea fisierului primit ca parametru */
size_t get_filesize(FILE *file)
{
    size_t current_pos = ftell(file);

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, current_pos, SEEK_SET);

    return size;
}

/* returneaza continutul fisierului ca sir de caractere */
char *get_string_from_file(FILE *file)
{
    size_t filesize = get_filesize(file);

    char *str = malloc((filesize + 1) * sizeof(char));
    if (str == NULL) {
        return NULL;
    }

    /* citeste continutul fisierului intr-un sir de caractere */
    size_t read_chars = fread(str, sizeof(char), filesize, file);
    if (read_chars != filesize) {
        return NULL;
    }

    str[filesize] = '\0';

    return str;
}

/* returneaza un obiect cJSON cu structura JSON din fisierul dat */
cJSON *get_json_from_file(FILE *file)
{
    char *json_string = get_string_from_file(file);
    if (json_string == NULL) {
        fprintf(stderr, "Couldn't get file string!\n");
        return NULL;
    }

    cJSON *json = cJSON_Parse(json_string);

    free(json_string);
    if (json == NULL) {
        fprintf(stderr, "Couldn't parse the JSON file!\n");
        return NULL;
    }

    return json;
}

/* obtine fileheader-ul din fisierul JSON si-l stocheaza */
bmp_fileheader *get_fileheader_from_json(cJSON *json)
{
    bmp_fileheader *fileheader = malloc(sizeof(bmp_fileheader));
    if (fileheader == NULL) {
        return NULL;
    }

    cJSON *fileheader_obj = cJSON_GetObjectItem(json, "file_header");
    if (fileheader_obj == NULL) {
        free(fileheader);

        return NULL;
    }

    cJSON *signature = cJSON_GetObjectItem(fileheader_obj, "signature");
    cJSON *file_size = cJSON_GetObjectItem(fileheader_obj, "file_size");
    cJSON *offset = cJSON_GetObjectItem(fileheader_obj, "offset");

    if (signature == NULL || file_size == NULL || offset == NULL) {
        free(fileheader);

        return NULL;
    }

    fileheader->fileMarker1 = signature->valuestring[0];
    fileheader->fileMarker2 = signature->valuestring[1];
    fileheader->bfSize = file_size->valueint;
    fileheader->unused1 = 0;
    fileheader->unused2 = 0;
    fileheader->imageDataOffset = offset->valueint;

    return fileheader;
}

/* obtine infoheader-ul din fisierul JSON si-l stocheaza */
bmp_infoheader *get_infoheader_from_json(cJSON *json)
{
    bmp_infoheader *infoheader = malloc(sizeof(bmp_infoheader));
    if (infoheader == NULL) {
        return NULL;
    }

    cJSON *infoheader_obj = cJSON_GetObjectItem(json, "info_header");
    if (infoheader_obj == NULL) {
        free(infoheader);

        return NULL;
    }

    cJSON *size = cJSON_GetObjectItem(infoheader_obj, "size");
    cJSON *width = cJSON_GetObjectItem(infoheader_obj, "width");
    cJSON *height = cJSON_GetObjectItem(infoheader_obj, "height");
    cJSON *planes = cJSON_GetObjectItem(infoheader_obj, "planes");
    cJSON *bit_count = cJSON_GetObjectItem(infoheader_obj, "bit_count");
    cJSON *compression = cJSON_GetObjectItem(infoheader_obj, "compression");
    cJSON *image_size = cJSON_GetObjectItem(infoheader_obj, "image_size");
    cJSON *x_pperm = cJSON_GetObjectItem(infoheader_obj, "x_pixels_per_meter");
    cJSON *y_pperm = cJSON_GetObjectItem(infoheader_obj, "y_pixels_per_meter");
    cJSON *clrs_used = cJSON_GetObjectItem(infoheader_obj, "colors_used");
    cJSON *clrs_imp = cJSON_GetObjectItem(infoheader_obj, "colors_important");

    if (!size || !width || !height || !planes || !bit_count || !compression ||
        !image_size || !x_pperm || !y_pperm || !clrs_used || !clrs_imp) {
        free(infoheader);

        return NULL;
    }

    infoheader->biSize = size->valueint;
    infoheader->width = width->valueint;
    infoheader->height = height->valueint;
    infoheader->planes = planes->valueint;
    infoheader->bitPix = bit_count->valueint;
    infoheader->biCompression = compression->valueint;
    infoheader->biSizeImage = image_size->valueint;
    infoheader->biXPelsPerMeter = x_pperm->valueint;
    infoheader->biYPelsPerMeter = y_pperm->valueint;
    infoheader->biClrUsed = clrs_used->valueint;
    infoheader->biClrImportant = clrs_imp->valueint;

    return infoheader;
}

/* obtine matricea de pixeli din fisierul JSON dat */
Color *get_pixel_array_from_json(cJSON *json)
{
    cJSON *colors = cJSON_GetObjectItem(json, "bitmap");
    if (colors == NULL) {
        return NULL;
    }

    Color *pixel_arr = malloc(cJSON_GetArraySize(colors) / 3 * sizeof(Color));
    if (pixel_arr == NULL) {
        return NULL;
    }

    size_t pixel_arr_index = 0;
    size_t colors_index = 0;

    cJSON *color = NULL;
    cJSON_ArrayForEach(color, colors) {
        if (colors_index % 3 == 0) {
            /* valoare pentru culoarea albastru */
            pixel_arr[pixel_arr_index].blue = color->valueint;
        } else if (colors_index % 3 == 1) {
            /* valoare pentru culoarea verde */
            pixel_arr[pixel_arr_index].green = color->valueint;
        } else {
            /* valoare pentru culoarea rosu */
            pixel_arr[pixel_arr_index].red = color->valueint;
            ++pixel_arr_index;
        }
        ++colors_index;
    }

    return pixel_arr;
}

/* returneaza numarul de octeti de padding ce trebuie adaugati */
unsigned int get_pixel_array_padding(bmp *bitmap)
{
    return (4 - bitmap->infoheader->width * 3 % 4) % 4;
}

/* obtine si returneaza structura bmp cu datele din JSON */
bmp *get_bmp_from_json(cJSON *json)
{
    bmp *bitmap = malloc(sizeof(bmp));
    if (bitmap == NULL) {
        fprintf(stderr, "Couldn't allocate memory for the bmp struct!\n");
        return NULL;
    }

    bitmap->fileheader = get_fileheader_from_json(json);
    if (bitmap->fileheader == NULL) {
        fprintf(stderr, "Couldn't get fileheader from JSON file!\n");
        return NULL;
    }

    bitmap->infoheader = get_infoheader_from_json(json);
    if (bitmap->infoheader == NULL) {
        fprintf(stderr, "Couldn't get infoheader from JSON file!\n");
        return NULL;
    }

    bitmap->pixel_array = get_pixel_array_from_json(json);
    if (bitmap->pixel_array == NULL) {
        fprintf(stderr, "Couldn't get pixel array from json file!\n");
        return NULL;
    }

    bitmap->padding = get_pixel_array_padding(bitmap);

    return bitmap;
}

/* dealoca memoria ocupata de structura bmp */
void free_bmp(bmp *bitmap)
{
    free(bitmap->fileheader);
    free(bitmap->infoheader);
    free(bitmap->pixel_array);

    free(bitmap);
}

/* returneaza numele fisierului din calea data (fara extensie) */
char *get_file_basename(char *filepath)
{
    char *filepath_copy = strdup(filepath);
    if (filepath_copy == NULL) {
        return NULL;
    }

    char *filename = strrchr(filepath_copy, '/') + 1;
    char *basename = strtok(filename, "."); /* inlaturare extensie */

    char *str = strdup(basename);

    free(filepath_copy);

    return str;
}

/* returneaza numele fisierului de output, in functie de task si input */
char *get_output_filename(unsigned int task, char *input_path)
{
    char *output_filename = malloc(OUTPUT_FILENAME_SIZE * sizeof(char));
    if (output_filename == NULL) {
        fprintf(stderr, "Couldn't allocate memory for output filename!\n");
        return NULL;
    }

    char *basename = get_file_basename(input_path);
    if (basename == NULL) {
        fprintf(stderr, "Couldn't get basename!\n");
        return NULL;
    }

    if (task == 3) {
        sprintf(output_filename, "output_task%d_%s.json", task, basename);
    } else {
        sprintf(output_filename, "output_task%d_%s.bmp", task, basename);
    }

    free(basename);

    return output_filename;
}

/* exporta structura bmp intr-un fisier BMP */
int write_to_bitmap_file(bmp *bitmap, char *input_path, unsigned int task)
{
    /* formare nume fisier de output */
    char *output_filename = get_output_filename(task, input_path);
    if (output_filename == NULL) {
        return 1;
    }

    /* deschidere fisier de output */
    FILE *output_file = fopen(output_filename, "wb");
    free(output_filename);
    if (output_file == NULL) {
        fprintf(stderr, "Couldn't open / create file %s!\n", output_filename);
        return 1;
    }

    /* scriere in fisierul de output */
    fwrite(bitmap->fileheader, sizeof(bmp_fileheader), 1, output_file);
    fwrite(bitmap->infoheader, sizeof(bmp_infoheader), 1, output_file);

    for (int i = bitmap->infoheader->height - 1; i >= 0; --i) {
        for (int j = 0; j < bitmap->infoheader->width; ++j) {
            fwrite(&(bitmap->pixel_array[i * bitmap->infoheader->width + j]),
                    sizeof(Color), 1, output_file);
        }

        if (bitmap->padding > 0) {
            char zero = 0;
            for (unsigned short i = 0; i < bitmap->padding; ++i) {
                fwrite(&zero, sizeof(char), 1, output_file);
            }
        }
    }

    fclose(output_file);

    return 0;
}

/* output task 3 */
int write_to_json_file(char *input_path, int state)
{
    /* creare structura fisier JSON */
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        fprintf(stderr, "Couldn't create a cJSON object!\n");
        return 1;
    }

    char *input_filename = strrchr(input_path, '/') + 1;
    cJSON *input_file = cJSON_CreateString(input_filename);
    if (input_file == NULL) {
        cJSON_Delete(json);

        fprintf(stderr, "Couldn't create a cJSON string!\n");
        return 1;
    }

    cJSON_AddItemToObject(json, "input_file", input_file);

    cJSON *game_state = NULL;
    if (state == 0) {
        game_state = cJSON_CreateString("Win!");
    } else {
        game_state = cJSON_CreateString("Loss :(");
    }

    if (game_state == NULL) {
        cJSON_Delete(json);

        fprintf(stderr, "Couldn't create a cJSON string!\n");
        return 1;
    }

    cJSON_AddItemToObject(json, "game_state", game_state);

    char *str = cJSON_Print(json);
    cJSON_Delete(json);
    if (str == NULL) {
        fprintf(stderr, "Couldn't render a cJSON entity to text!\n");
        return 1;
    }

    /* formare nume fisier de output pentru task 3 */
    char *output_filename = get_output_filename(3, input_path);
    if (output_filename == NULL) {
        return 1;
    }

    /* deschidere fisier de output pentru task 3 */
    FILE *output_file = fopen(output_filename, "w");
    free(output_filename);
    if (output_file == NULL) {
        fprintf(stderr, "Couldn't open / create file %s!\n", output_filename);
        return 1;
    }

    fprintf(output_file, "%s\n", str);

    fclose(output_file);
    free(str);

    return 0;
}
