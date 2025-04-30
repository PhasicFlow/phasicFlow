#!/usr/bin/env python3

import os
import re
from bs4 import BeautifulSoup
import os.path

# Configuration: map of source README files to destination wiki pages
FILE_MAPPINGS = {
    'benchmarks/rotatingDrum/readme.md': 'Performance-of-phasicFlow.md',
    # Add more mappings as needed
}

def convert_relative_links(content, source_path, repo_name):
    """Convert relative links to absolute GitHub links"""
    repo_base_url = f"https://github.com/{repo_name}/blob/main/"
    
    # Find the directory of the source file to correctly resolve relative paths
    source_dir = os.path.dirname(source_path)
    if source_dir:
        source_dir += '/'
        
    # Convert Markdown links: [text](link)
    def replace_md_link(match):
        link_text = match.group(1)
        link_path = match.group(2)
        
        # Skip links that are already absolute
        if link_path.startswith(('http://', 'https://', '#')):
            return f"[{link_text}]({link_path})"
        
        # Convert relative path to absolute
        if link_path.startswith('./'):
            link_path = link_path[2:]
        elif link_path.startswith('../'):
            # Need to resolve the path based on source_dir
            path_parts = source_dir.strip('/').split('/')
            current_path = link_path
            while current_path.startswith('../'):
                path_parts.pop()
                current_path = current_path[3:]
            new_path = '/'.join(path_parts) + '/' + current_path if path_parts else current_path
            return f"[{link_text}]({repo_base_url}{new_path})"
            
        absolute_path = f"{source_dir}{link_path}" if not link_path.startswith('/') else link_path[1:]
        return f"[{link_text}]({repo_base_url}{absolute_path})"
        
    content = re.sub(r'\[([^\]]+)\]\(([^)]+)\)', replace_md_link, content)
    
    # Preserve HTML img tags with their style attributes and fix src paths
    soup = BeautifulSoup(content, 'html.parser')
    for img in soup.find_all('img'):
        if img.get('src') and not img['src'].startswith(('http://', 'https://')):
            src = img['src']
            if src.startswith('./'):
                src = src[2:]
            if not src.startswith('/'):
                src = f"{source_dir}{src}"
            else:
                src = src[1:]  # Remove leading slash
            
            img['src'] = f"{repo_base_url}{src}"
    
    # Convert the modified HTML back to string, but only if we found any img tags
    if soup.find_all('img'):
        # Extract just the modified HTML parts and reinsert them
        for img in soup.find_all('img'):
            img_str = str(img)
            # Find the original img tag in content and replace it
            original_img_pattern = re.compile(r'<img[^>]*src=["\']([^"\']*)["\'][^>]*>')
            for match in original_img_pattern.finditer(content):
                orig_src = match.group(1)
                if orig_src in img['src'] or img['src'].endswith(orig_src):
                    content = content.replace(match.group(0), img_str)
    
    return content

def main():
    repo_name = os.environ.get('GITHUB_REPOSITORY', 'PhasicFlow/phasicFlow')
    repo_path = os.path.join(os.environ.get('GITHUB_WORKSPACE', '.'), 'repo')
    wiki_path = os.path.join(os.environ.get('GITHUB_WORKSPACE', '.'), 'wiki')
    
    for source_rel_path, dest_wiki_page in FILE_MAPPINGS.items():
        source_path = os.path.join(repo_path, source_rel_path)
        dest_path = os.path.join(wiki_path, dest_wiki_page)
        
        print(f"Processing: {source_path} -> {dest_path}")
        
        if not os.path.exists(source_path):
            print(f"Warning: Source file {source_path} does not exist, skipping.")
            continue
            
        # Read the source file
        with open(source_path, 'r', encoding='utf-8') as f:
            content = f.read()
            
        # Convert relative links to absolute
        content = convert_relative_links(content, source_rel_path, repo_name)
            
        # Write the modified content to the destination
        with open(dest_path, 'w', encoding='utf-8') as f:
            f.write(content)
            
        print(f"Successfully synced {source_path} to {dest_path}")

if __name__ == "__main__":
    main()
