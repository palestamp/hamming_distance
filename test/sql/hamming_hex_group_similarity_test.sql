SELECT hamming_similarity('0000', '0000');
SELECT hamming_similarity('0000', 'ffff');
SELECT hamming_similarity('ffff', '0000');
SELECT hamming_similarity('1201', '0101');
SELECT hamming_similarity('01010101', '10101010');
SELECT hamming_similarity('010101010101010101010101010101010101010101010101', '101010101010101010101010101010101010101010101010');
SELECT hamming_similarity('ffff', 'FFFF');
