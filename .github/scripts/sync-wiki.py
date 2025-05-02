#!/usr/bin/env python3

import os
import re
import yaml
import sys

# Constants
REPO_URL = "https://github.com/PhasicFlow/phasicFlow"
REPO_PATH = os.path.join(os.environ.get("GITHUB_WORKSPACE", ""), "repo")
WIKI_PATH = os.path.join(os.environ.get("GITHUB_WORKSPACE", ""), "wiki")
MAPPING_FILE = os.path.join(REPO_PATH, ".github/workflows/markdownList.yml")

def load_mapping():
    """Load the markdown to wiki page mapping file."""
    try:
        with open(MAPPING_FILE, 'r') as f:
            data = yaml.safe_load(f)
            return data.get('mappings', [])
    except Exception as e:
        print(f"Error loading mapping file: {e}")
        return []

def convert_relative_links(content, source_path):
    """Convert relative links in markdown content to absolute URLs."""
    # Find markdown links with regex pattern [text](url)
    md_pattern = r'\[([^\]]+)\]\(([^)]+)\)'
    
    # Find HTML img tags
    img_pattern = r'<img\s+src=[\'"]([^\'"]+)[\'"]'
    
    def replace_link(match):
        link_text = match.group(1)
        link_url = match.group(2)
        
        # Skip if already absolute URL or anchor
        if link_url.startswith(('http://', 'https://', '#', 'mailto:')):
            return match.group(0)
        
        # Get the directory of the source file
        source_dir = os.path.dirname(source_path)
        
        # Create absolute path from repository root
        if link_url.startswith('/'):
            # If link starts with /, it's already relative to repo root
            abs_path = link_url
        else:
            # Otherwise, it's relative to the file location
            abs_path = os.path.normpath(os.path.join(source_dir, link_url))
            if not abs_path.startswith('/'):
                abs_path = '/' + abs_path
        
        # Convert to GitHub URL
        github_url = f"{REPO_URL}/blob/main{abs_path}"
        return f"[{link_text}]({github_url})"
    
    def replace_img_src(match):
        img_src = match.group(1)
        
        # Skip if already absolute URL
        if img_src.startswith(('http://', 'https://')):
            return match.group(0)
        
        # Get the directory of the source file
        source_dir = os.path.dirname(source_path)
        
        # Create absolute path from repository root
        if img_src.startswith('/'):
            # If link starts with /, it's already relative to repo root
            abs_path = img_src
        else:
            # Otherwise, it's relative to the file location
            abs_path = os.path.normpath(os.path.join(source_dir, img_src))
            if not abs_path.startswith('/'):
                abs_path = '/' + abs_path
        
        # Convert to GitHub URL (use raw URL for images)
        github_url = f"{REPO_URL}/raw/main{abs_path}"
        return f'<img src="{github_url}"'
    
    # Replace all markdown links
    content = re.sub(md_pattern, replace_link, content)
    
    # Replace all img src tags
    content = re.sub(img_pattern, replace_img_src, content)
    
    return content

def process_file(source_file, target_wiki_page):
    """Process a markdown file and copy its contents to a wiki page."""
    source_path = os.path.join(REPO_PATH, source_file)
    target_path = os.path.join(WIKI_PATH, f"{target_wiki_page}.md")
    
    print(f"Processing {source_path} -> {target_path}")
    
    try:
        # Check if source exists
        if not os.path.exists(source_path):
            print(f"Source file not found: {source_path}")
            return False
        
        # Read source content
        with open(source_path, 'r') as f:
            content = f.read()
        
        # Convert relative links
        content = convert_relative_links(content, source_file)
        
        # Write to wiki page
        with open(target_path, 'w') as f:
            f.write(content)
        
        return True
    
    except Exception as e:
        print(f"Error processing {source_file}: {e}")
        return False

def main():
    # Check if wiki directory exists
    if not os.path.exists(WIKI_PATH):
        print(f"Wiki path not found: {WIKI_PATH}")
        sys.exit(1)
    
    # Load mapping
    mappings = load_mapping()
    if not mappings:
        print("No mappings found in the mapping file")
        sys.exit(1)
    
    print(f"Found {len(mappings)} mappings to process")
    
    # Process each mapping
    success_count = 0
    for mapping in mappings:
        source = mapping.get('source')
        target = mapping.get('target')
        
        if not source or not target:
            print(f"Invalid mapping: {mapping}")
            continue
        
        if process_file(source, target):
            success_count += 1
    
    print(f"Successfully processed {success_count} of {len(mappings)} files")
    
    # Exit with error if any file failed
    if success_count < len(mappings):
        sys.exit(1)

if __name__ == "__main__":
    main()
