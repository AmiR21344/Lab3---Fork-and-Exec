CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -g
BINDIR = ../build

# List all challenge programs
CHALLENGES = ch1_single_fork ch2_multi_child ch3_exec_ls worker ch4_exec_worker \
             ch5_exec_examples ch6_pipe_sum ch7_pipeline ch8_wait_nonblock \
             ch9_zombie ch10_pool

all: $(addprefix $(BINDIR)/, $(CHALLENGES))

$(BINDIR)/%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(addprefix $(BINDIR)/, $(CHALLENGES))

test: all
	@echo "Running challenge tests..."
	@for prog in $(CHALLENGES); do \
		if [ -f "$(BINDIR)/$$prog" ]; then \
			echo "=== Testing $$prog ==="; \
			$(BINDIR)/$$prog 2>&1 | head -10; \
			echo ""; \
		fi \
	done

.PHONY: all clean test
