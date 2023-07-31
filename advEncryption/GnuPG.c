#include <stdio.h>
#include <gpgme.h>
static void handle_gpgme_error(gpgme_error_t err) {
    fprintf(stderr, "Error: %s\n", gpgme_strerror(err));
}
int gpg_encrypt_file(const char *input_filename, const char *output_filename, const char *recipient) {
    gpgme_ctx_t ctx;
    gpgme_error_t err;
    FILE *input_file, *output_file;
    gpgme_key_t key[2] = { NULL, NULL };
    gpgme_check_version(NULL);
    gpgme_set_locale(NULL, LC_CTYPE, setlocale(LC_CTYPE, NULL));
    gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP);
    err = gpgme_new(&ctx);
    if (err) {
        handle_gpgme_error(err);
        return -1;
    }
    gpgme_set_protocol(ctx, GPGME_PROTOCOL_OpenPGP);
    err = gpgme_get_key(ctx, recipient, &key[0], 0);
    if (err) {
        handle_gpgme_error(err);
        gpgme_release(ctx);
        return -1;
    }
    input_file = fopen(input_filename, "rb");
    output_file = fopen(output_filename, "wb");
    if (!input_file || !output_file) {
        perror("Error opening file");
        gpgme_key_unref(key[0]);
        gpgme_release(ctx);
        return -1;
    }
    gpgme_data_t input, output;
    gpgme_data_new_from_stream(&input, input_file);
    gpgme_data_new(&output);

    err = gpgme_op_encrypt(ctx, key, GPGME_ENCRYPT_ALWAYS_TRUST, input, output);
    if (err) {
        handle_gpgme_error(err);
        gpgme_data_release(input);
        gpgme_data_release(output);
        gpgme_key_unref(key[0]);
        gpgme_release(ctx);
        fclose(input_file);
        fclose(output_file);
        return -1;
    }
    ssize_t bytes_written;
    const void *data = gpgme_data_release_and_get_mem(output, &bytes_written);
    fwrite(data, 1, bytes_written, output_file);
    gpgme_data_release(input);
    gpgme_free(data);
    gpgme_key_unref(key[0]);
    gpgme_release(ctx);
    fclose(input_file);
    fclose(output_file);

    return 0;
}
int gpg_decrypt_file(const char *input_filename, const char *output_filename) {
    gpgme_ctx_t ctx;
    gpgme_error_t err;
    FILE *input_file, *output_file;
    gpgme_check_version(NULL);
    gpgme_set_locale(NULL, LC_CTYPE, setlocale(LC_CTYPE, NULL));
    gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP);
    err = gpgme_new(&ctx);
    if (err) {
        handle_gpgme_error(err);
        return -1;
    }
    gpgme_set_protocol(ctx, GPGME_PROTOCOL_OpenPGP);
    input_file = fopen(input_filename, "rb");
    output_file = fopen(output_filename, "wb");
    if (!input_file || !output_file) {
        perror("Error opening file");
        gpgme_release(ctx);
        return -1;
    }
    gpgme_data_t input, output;
    gpgme_data_new_from_stream(&input, input_file);
    gpgme_data_new(&output);

    err = gpgme_op_decrypt(ctx, input, output);
    if (err) {
        handle_gpgme_error(err);
        gpgme_data_release(input);
        gpgme_data_release(output);
        gpgme_release(ctx);
        fclose(input_file);
        fclose(output_file);
        return -1;
    }
    ssize_t bytes_written;
    const void *data = gpgme_data_release_and_get_mem(output, &bytes_written);
    fwrite(data, 1, bytes_written, output_file);
    gpgme_data_release(input);
    gpgme_free(data);
    gpgme_release(ctx);
    fclose(input_file);
    fclose(output_file);

    return 0;
}
int main() {
    const char *input_file = "input.txt";
    const char *encrypted_file = "encrypted.gpg";
    const char *decrypted_file = "decrypted.txt";
    const char *recipient = "recipient@example.com";
    if (gpg_encrypt_file(input_file, encrypted_file, recipient) == 0) {
        printf("Encryption successful.\n");
    } else {
        printf("Encryption failed.\n");
    }
    if (gpg_decrypt_file(encrypted_file, decrypted_file) == 0) {
        printf("Decryption successful.\n");
    } else {
        printf("Decryption failed.\n");
    }
    return 0;
}
