#!/bin/bash
# Re-encode all BMP files to standard format

for file in *.bmp; do
    if [ -f "$file" ]; then
        echo "Processing: $file"
        # Create temporary file
        ffmpeg -i "$file" -pix_fmt bgr24 -y "temp_${file}"
        
        # Check if conversion was successful
        if [ $? -eq 0 ]; then
            # Replace original with converted file
            mv "temp_${file}" "$file"
            echo "✓ Successfully converted: $file"
        else
            echo "✗ Failed to convert: $file"
            rm -f "temp_${file}"
        fi
    fi
done

echo "All BMP files have been processed!"
