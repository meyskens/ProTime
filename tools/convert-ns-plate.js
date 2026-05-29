const sharp = require('sharp');
const fs = require('fs');
const path = require('path');

// Read the ns-plate.png file
const inputPath = path.join(__dirname, '..', 'ns-plate.png');
const outputDir = path.join(__dirname, '..', 'resources', 'images');

// Platform screen sizes from hardware information:
// - Aplite: 144x168, BW, Rectangle
// - Basalt: 144x168, Color, Rectangle
// - Chalk: 180x180, Color, Round
// - Diorite: 144x168, BW, Rectangle
// - Emery: 200x228, Color, Rectangle
// - Flint: 200x228, Color, Round
// - Gabbro: 260x260, Color, Round
const sizes = [
  // Aplite: 144x168, BW
  { name: 'ns-face-round-aplite', width: 144, height: 168, fit: 'contain' },
  // Basalt: 144x168, Color
  { name: 'ns-face-round-color', width: 144, height: 168, fit: 'contain' },
  // Chalk: 180x180, Round, Color
  { name: 'ns-face-round-chalk', width: 180, height: 180, fit: 'contain' },
  // Diorite: 144x168, BW
  { name: 'ns-face-round-diorite', width: 144, height: 168, fit: 'contain' },
  // Emery: 200x228, Color
  { name: 'ns-face-round-emery', width: 200, height: 228, fit: 'contain' },
  // Flint: 144 x 168, BW
  { name: 'round-flint', width: 144, height: 168, fit: 'contain' },
  // Gabbro: 260x260, Color, Round
  { name: 'ns-face-round-gabbro', width: 260, height: 260, fit: 'contain' },
];

// Logo variants use the same sizes
const logoSizes = [
  { name: 'ns-face-round-logo-aplite', width: 144, height: 168, fit: 'contain' },
  { name: 'ns-face-round-logo-color', width: 144, height: 168, fit: 'contain' },
  { name: 'ns-face-round-logo-chalk', width: 180, height: 180, fit: 'contain' },
  { name: 'ns-face-round-logo-diorite', width: 144, height: 168, fit: 'contain' },
  { name: 'ns-face-round-logo-emery', width: 200, height: 228, fit: 'contain' },
  { name: 'ns-face-round-logo-flint', width: 144, height: 168, fit: 'contain' },
  { name: 'ns-face-round-logo-gabbro', width: 260, height: 260, fit: 'contain' },
];

async function convert() {
  // Check if input file exists
  if (!fs.existsSync(inputPath)) {
    console.error(`Error: ${inputPath} not found!`);
    process.exit(1);
  }

  const imageBuffer = fs.readFileSync(inputPath);

  // Convert standard face plates
  for (const size of sizes) {
    const outputPath = path.join(outputDir, `${size.name}.png`);
    await sharp(imageBuffer)
      .resize(size.width, size.height, { fit: size.fit, background: { r: 255, g: 255, b: 255, alpha: 0 } })
      .png()
      .toFile(outputPath);
    console.log(`Created ${size.name}.png`);
  }

  // Convert logo face plates
  const logoInputPath = path.join(__dirname, '..', 'ns-plate-logo.png');
  if (fs.existsSync(logoInputPath)) {
    const logoBuffer = fs.readFileSync(logoInputPath);
    for (const size of logoSizes) {
      const outputPath = path.join(outputDir, `${size.name}.png`);
      await sharp(logoBuffer)
        .resize(size.width, size.height, { fit: size.fit, background: { r: 255, g: 255, b: 255, alpha: 0 } })
        .png()
        .toFile(outputPath);
      console.log(`Created ${size.name}.png`);
    }
  } else {
    console.log('Note: ns-plate-logo.png not found, skipping logo variants');
  }

  console.log('NS plate conversion complete!');
}

convert().catch(console.error);
