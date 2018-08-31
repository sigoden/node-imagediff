const fs = require('fs');

const { diffFactor, diffImage } = require('..');

if (process.argv.length !== 5) {
  console.log('Usage diff <img1> <img2> <dest>');
  process.exit(1);
}

const img1 = fs.readFileSync(process.argv[2]);
const img2 = fs.readFileSync(process.argv[3]);
const factor = diffFactor(img1, img2);
console.log(factor);

const imgDiff = diffImage(img1, img2);
fs.writeFileSync(process.argv[4], imgDiff);