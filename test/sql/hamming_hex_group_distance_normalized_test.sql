SELECT hamming_distance_normalized('0000', '0000');
SELECT hamming_distance_normalized('0000', 'ffff');
SELECT hamming_distance_normalized('ffff', '0000');
SELECT hamming_distance_normalized('1201', '0101');
SELECT hamming_distance_normalized('01010101', '10101010');
SELECT hamming_distance_normalized('010101010101010101010101010101010101010101010101', '101010101010101010101010101010101010101010101010');
SELECT hamming_distance_normalized('ffff', 'FFFF');
