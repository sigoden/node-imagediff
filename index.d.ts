import { Buffer } from "buffer";

type Opaque<T, K> = T & { __opaque__: K };
type Int = Opaque<number, "Int">;

/**
 * Diff two image
 * @param img1 - The img
 * @param img2 - Anthor img
 * @param colorThred - The color threshold, 0 - 255
 */
export function diff(img1: Buffer, img2: Buffer, colorThred: Int): boolean;

// Options to control the encoding of diff image
interface Encoding {
  // Format of the diff image, use extname `.jpg`, `.png` to repersent
  ext?: string;
  /**
   * Quantity of the diff image
   * For JPEG, it can be a quality ( CV_IMWRITE_JPEG_QUALITY ) from 0 to 100 (the higher is the better). Default value is 95.
   * For PNG, it can be the compression level ( CV_IMWRITE_PNG_COMPRESSION ) from 0 to 9. A higher value means a smaller size and longer compression time. Default value is 3.
   * For WEBP, it can be a quality ( CV_IMWRITE_WEBP_QUALITY ) from 1 to 100 (the higher is the better). By default (without any parameter) and for quality above 100 the lossless compression is used.
   */
  quantity?: Int;
}

/**
 * Get difference of two images
 * @param img1 - The img
 * @param img2 - Anthor img
 * @param encoding -
 */
export function getDiff(
  img1: Buffer,
  img2: Buffer,
  encoding?: Encoding
): Buffer;
