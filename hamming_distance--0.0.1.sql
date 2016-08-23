\echo Use "CREATE EXTENSION hamming_distance" to load this file. \quit
CREATE OR REPLACE FUNCTION hamming_distance(varchar, varchar) RETURNS int
AS '$libdir/hamming_distance', 'hamming_distance'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION hamming_similarity(varchar, varchar) RETURNS int
AS '$libdir/hamming_distance', 'hamming_similarity'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION hamming_distance_normalized(varchar, varchar) RETURNS float
AS '$libdir/hamming_distance', 'hamming_distance_normalized'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION hamming_similarity_normalized(varchar, varchar) RETURNS float
AS '$libdir/hamming_distance', 'hamming_similarity_normalized'
LANGUAGE C IMMUTABLE STRICT;
