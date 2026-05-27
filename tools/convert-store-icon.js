const sharp = require('sharp');
const fs = require('fs');
const path = require('path');

// Read the SVG file
const svgPath = path.join(__dirname, 'ns.svg');
const svgBuffer = fs.readFileSync(svgPath);

// Pebble Store icons: 80x80 and 144x144
// NS Yellow background color: #ffc917
const sizes = [
  { name: 'store-icon-80', width: 80, height: 80 },
  { name: 'store-icon-144', width: 144, height: 144 },
];

// NS Yellow #ffc917 in RGB
const backgroundColor = { r: 255, g: 201, b: 23, alpha: 1 };

async function convert() {
  for (const size of sizes) {
    const outputPath = path.join(__dirname, '..', 'resources', 'images', `${size.name}.png`);

    await sharp(svgBuffer)
      .resize(size.width, size.height, { fit: 'contain', background: backgroundColor })
      .flatten({ background: backgroundColor })
      .png()
      .toFile(outputPath);

    console.log(`Created ${size.name}.png at ${outputPath}`);
  }
  console.log('Store icon conversion complete!');
}

convert().catch(console.error);
