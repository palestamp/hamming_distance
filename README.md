Hex-Pair Hamming Distance Functions
===================================
This extension provides four functions to work with hex representation of 
feature vector.

    

Every function asserts that both arguments have the same length.



### hamming_distance(varchar, varchar) RETURNS int (IMMUTABLE STRICT)

Computes absolute hamming distance between to FVs

    SELECT hamming_distance('1d3f', '1110');
     hamming_distance
    ------------------
                    7
    (1 row)

This expands as

    a = '1d3f'  -> 0001 1101 0011 1111
    b = '1110'  -> 0001 0001 0001 0000
 
    c = a ^ b   -> 0000 1100 0010 1111

    popcount(c) ->   0 + 2 + 1 + 4 = 7
    

### hamming_similarity(varchar, varchar) RETURNS int (IMMUTABLE STRICT)

    SELECT hamming_similarity('1d3f', '1110');
     hamming_similarity
    --------------------
                      9
    (1 row)

Can be defined as
    
    HAMMING_SIMILARITY = len(a) * 4 - hamming_distance(a, b);


### hamming_distance_normalized(varchar, varchar) RETURNS float (IMMUTABLE STRICT)

    SELECT hamming_distance_normalized('1d3f', '1110');
     hamming_distance_normalized
    -----------------------------
                          0.4375
    (1 row)

Can be defined as
    
    HAMMING_DISTANCE_NORM = hamming_distance(a, b) / len(a) * 4;


### hamming_similarity_normalized(varchar, varchar) RETURNS float (IMMUTABLE STRICT)

    SELECT hamming_similarity_normalized('1d3f', '1110');
     hamming_similarity_normalized
    -------------------------------
                            0.5625
    (1 row)

Can be defined as
    
    HAMMING_SIMILARITY_NORM = hamming_similarity(a, b) / len(a) * 4;


Installation
============

    cd .../hamming_distance
    sudo make install


Running tests
=============
    
    make installcheck 
