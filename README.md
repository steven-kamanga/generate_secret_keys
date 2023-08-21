# Random Key Generator

## Key Generator

The Random Key Generator is a fun and interactive command-line tool that allows you to create secure random keys by mixing characters from a specified range and adding a unique salt.

## Usage

To use the Random Key Generator, follow the usage instructions below:

```bash
Usage: generate -l <length> -f <from> -t <to> -o <output_path>
```

Replace the placeholders with the appropriate values for your desired key generation. You can set the length of the key, specify the range of characters to choose from, and set the output location.

## Getting Started

1. **Installation**: Make sure you have OpenSSL installed on your system. If not, install it using your package manager.

2. **Compilation**: Compile the code files using the following command:

   ```bash
   gcc main.c generator.c file_writer.c -o key_generator
   ```

3. **Run**: Execute the generated binary to use the Random Key Generator.

## Options

- `-l <int:length>`: Set the length of the generated key.
- `-f <string:from>`: Specify the starting character for the range.
- `-t <string:to>`: Specify the ending character for the range.
- `-o </home/user/...txt>`: Set the output location for the generated key.

## Example

Generate a random key with a length of 20 characters, using characters ranging from 'a' to 'z', and save it to `/home/user/key.txt`:

```bash
./key_generator -l 20 -f a -t z -o /home/user/key.txt
```

## Features

- Generates secure random keys using a specified range of characters.
- Customizable key length, character range, and output location.

## Note

The Random Key Generator is a simple and fun project that showcases the usage of cryptographic concepts to enhance key security. It's important to keep security practices in mind when using keys for sensitive applications.
```
