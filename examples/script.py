import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from pathlib import Path

# Define the image files
rows = ['1', '2', '3']
cols = ['bin', 'bing', 'binp', 'tex', 'texg', 'texp']

# Create figure with subplots
fig, axes = plt.subplots(3, 6, figsize=(20, 10))

# Make background transparent
fig.patch.set_alpha(0)

# Load and display images
for i, row in enumerate(rows):
    for j, col in enumerate(cols):
        img_path = f'{row}-{col}.bmp'
        
        try:
            img = mpimg.imread(img_path)
            axes[i, j].imshow(img)
        except FileNotFoundError:
            print(f"Not found: {img_path}")
            axes[i, j].text(0.5, 0.5, 'Not found', 
                          ha='center', va='center', fontsize=8)
        
        # Remove all axes
        axes[i, j].axis('off')
        
        # Make subplot background transparent
        axes[i, j].patch.set_alpha(0)

# Adjust layout with margins
plt.subplots_adjust(left=0.02, right=0.98, top=0.98, bottom=0.02, 
                    wspace=0.05, hspace=0.05)

# Save the figure with transparency
plt.savefig('results_comparison.png', dpi=150, bbox_inches='tight', 
            transparent=True, pad_inches=0.1)
plt.close()
print("Image saved as 'results_comparison.png'")
