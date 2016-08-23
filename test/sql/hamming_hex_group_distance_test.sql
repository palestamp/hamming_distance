SELECT hamming_distance('0000', '0000');
SELECT hamming_distance('0000', 'ffff');
SELECT hamming_distance('ffff', '0000');
SELECT hamming_distance('1201', '0101');
SELECT hamming_distance('01010101', '10101010');
SELECT hamming_distance('010101010101010101010101010101010101010101010101', '101010101010101010101010101010101010101010101010');
SELECT hamming_distance('ffff', 'FFFF');
