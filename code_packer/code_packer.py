import os
import argparse

# Configuration: Folders and file extensions to ignore
IGNORE_DIRS = {'.git', 'node_modules', '__pycache__', 'venv', 'env', '.idea', '.vscode', 'dist', 'build', 'assets', 'glad', 'glfw', 'glm', 'imgui', 'stb_image', 'stb_perlin'}
IGNORE_EXT = {'.png', '.jpg', '.jpeg', '.gif', '.exe', '.bin', '.pyc', '.zip', '.tar', '.gz', '.iso'}

def generate_tree(start_path):
    """Generates a visual directory tree string."""
    tree_str = f"Project Structure ({start_path}):\n"
    for root, dirs, files in os.walk(start_path):
        dirs[:] = [d for d in dirs if d not in IGNORE_DIRS]
        
        level = root.replace(start_path, '').count(os.sep)
        indent = ' ' * 4 * (level)
        tree_str += f"{indent}{os.path.basename(root)}/\n"
        subindent = ' ' * 4 * (level + 1)
        for f in files:
            if not any(f.endswith(ext) for ext in IGNORE_EXT):
                tree_str += f"{subindent}{f}\n"
    return tree_str

def is_text_file(filepath):
    """Simple check to see if a file is readable text."""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            f.read(1024)
        return True
    except (UnicodeDecodeError, IOError):
        return False

def pack_directory(source_dir, max_chars):
    """Reads files and packs them into chunks based on max_chars."""
    
    project_tree = generate_tree(source_dir)
    
    current_part = 1
    current_content = ""
    
    # Add the tree structure to the first file prompt
    intro_prompt = (
        "Below is a code repository. I will provide the directory structure "
        "followed by the file contents.\n\n"
        f"{project_tree}\n"
        "================================================================\n\n"
    )
    current_content += intro_prompt

    for root, dirs, files in os.walk(source_dir):
        dirs[:] = [d for d in dirs if d not in IGNORE_DIRS]
        
        for file in files:
            if any(file.endswith(ext) for ext in IGNORE_EXT):
                continue
            
            file_path = os.path.join(root, file)
            
            if not is_text_file(file_path):
                continue
                
            try:
                with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                    file_text = f.read()
                    
                file_header = f"\n\n--- START FILE: {file_path} ---\n"
                file_footer = f"\n--- END FILE: {file_path} ---\n"
                full_block = file_header + file_text + file_footer
                
                # 3. Check Size limits
                if len(current_content) + len(full_block) > max_chars:
                    output_filename = f"code_context_part_{current_part}.txt"
                    with open(output_filename, "w", encoding="utf-8") as out:
                        out.write(current_content)
                    print(f"Created {output_filename} ({len(current_content)} chars)")
                    
                    current_part += 1
                    current_content = f"Part {current_part} of the code repository...\n" + full_block
                else:
                    current_content += full_block
                    
            except Exception as e:
                print(f"Skipping {file_path}: {e}")

    # 4. Save the final remaining chunk
    if current_content:
        output_filename = f"code_context_part_{current_part}.txt"
        with open(output_filename, "w", encoding="utf-8") as out:
            out.write(current_content)
        print(f"Created {output_filename} ({len(current_content)} chars)")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Pack a directory into text files for AI context.")
    parser.add_argument("path", help="Path to the directory to pack")
    # Default is roughly 500k chars (approx 120k tokens), good for Gemini
    parser.add_argument("--max", type=int, default=500000, help="Max characters per output file")
    
    args = parser.parse_args()
    
    if os.path.isdir(args.path):
        pack_directory(args.path, args.max)
    else:
        print("Invalid directory path provided.")