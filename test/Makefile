BUILD := test/.build
# hack for .. paths
BUILD_OBJS := $(BUILD)/mirror

RUNNER := $(BUILD)/test-runner

CFLAGS :=

CPPFLAGS :=
CPPFLAGS += -Wall -Wextra -Werror -Wpedantic
CPPFLAGS += -g
CPPFLAGS += -pthread
CPPFLAGS += -Iinclude
CPPFLAGS += -Itest/ghost/include

LDFLAGS :=

# TODO these don't work yet, still under construction
ifeq ($(GOOGLETEST),1)
CPPFLAGS += -DGOOGLETEST
LDFLAGS += -lgtest -lgtest_main
CC=g++
else ifeq ($(CRITERION),1)
CPPFLAGS += -DCRITERION
LDFLAGS += -lcriterion
endif

SRCS := \
	$(shell find test/src -type f -name '*.c')
OBJS := $(patsubst %, $(BUILD_OBJS)/%.o, $(SRCS))

.PHONY: check
check: $(RUNNER)
	./$(RUNNER)

# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/#depdelete
CPPFLAGS += -MMD -MP
-include $(OBJS:.o=.d)

$(RUNNER): $(OBJS)
	mkdir -p $(dir $@)
	$(CC) -o $@ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^

$(OBJS): $(BUILD_OBJS)/%.o: % test/Makefile
	mkdir -p $(dir $@)
	$(CC) -o $@ -c $(CFLAGS) $(CPPFLAGS) $<

.PHONY: clean
clean:
	rm -rf test/.build
