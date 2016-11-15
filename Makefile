
ifndef REGRESS_DB_PORT
	REGRESS_DB_PORT = 5432
endif

EXTENSION     = hamming_distance
DATA          = $(wildcard $(EXTENSION)--*.sql)
TESTS         = $(wildcard test/sql/*.sql)

REGRESS_OPTS  = --port=$(REGRESS_DB_PORT)       \
	            --inputdir=test                 \
				--load-extension=$(EXTENSION)   \
				--load-language=plpgsql
REGRESS       = $(patsubst test/sql/%.sql,%,$(TESTS))

OBJS          = $(patsubst %.c,%.o,$(wildcard src/*.c))
MODULE_big    = $(EXTENSION)

PG_CONFIG     = pg_config
PSQL          = $(shell $(PG_CONFIG) --bindir)/psql


PGXS          = $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

