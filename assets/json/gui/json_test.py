import json
import os
import random

# -------- SETTINGS --------
output_file = "buttons.json"
print("Type how many buttons to create: ")
num_buttons = int(input())  # Change this to however many you want
base_name = "button"
# --------------------------

print("Starting")

button_texts = [
    "A",
    "B",
    "C",
    "D",
    "E",
]

# Load existing JSON if it exists
if os.path.exists(output_file):
    with open(output_file, "r") as f:
        data = json.load(f)
else:
    data = {}

# Generate buttons
for i in range(1, num_buttons + 1):
    button_name = f"{base_name}_{i}"

    data[button_name] = {
        "x": float(random.randrange(0,1920)),
        "y": float(random.randrange(0,1080)),
        "scale": 4.0,
        "size_x": float(random.randrange(16,64)),
        "size_y": float(random.randrange(16,64)),
        "text": button_texts[random.randrange(0,len(button_texts) - 1)]
    }

# Save back to file
with open(output_file, "w") as f:
    json.dump(data, f, indent=2)

print(f"Successfully added {num_buttons} buttons to {output_file}")