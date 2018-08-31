# diff image utils

Diff image to check whether objects have changed.

> Depends on [SSIM](https://en.wikipedia.org/wiki/Structural_similarity) and opencv.

## Install

**install opencv 3.x**

For linux

```
git clone https://github.com/opencv/opencv
mkdir opencv/build
cd opencv/build
cmake ..
sudo make install
```

For mac

```
brew tap homebrew/science
brew install opencv@3
brew link --force opencv@3
```

**install npm package**

```
npm install @sigodenjs/imagediff
```

## Usage

```js
const { diffFactor, diffImage } = require('..');

const img1 = fs.readFileSync("img1.jpg");
const img2 = fs.readFileSync("img2.jpg");
const factor = diffFactor(img1, img2);
const img = diffImage(img1, img2);
fs.writeFileSync("/tmp/imgdiff.jpg", img);
```

| img1                        | img2                        | diff                       | score |
| --------------------------- | --------------------------- | -------------------------- | ----- |
| ![pic1](https://raw.githubusercontent.com/sigoden/node-imagediff/master/example/pic1.jpg)   | ![pic2](https://raw.githubusercontent.com/sigoden/node-imagediff/master/example/pic2.jpg)   | ![pic1](https://raw.githubusercontent.com/sigoden/node-imagediff/master/doc/picdiff.jpg)   | 0.047 |
| ![view1](https://raw.githubusercontent.com/sigoden/node-imagediff/master/example/view1.jpg) | ![view2](https://raw.githubusercontent.com/sigoden/node-imagediff/master/example/view2.jpg) | ![view1](https://raw.githubusercontent.com/sigoden/node-imagediff/master/doc/viewdiff.jpg) | 0     |

## License

Copyright (c) 2018 sigoden

Licensed under the MIT license.