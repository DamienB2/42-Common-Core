PYTHON  = uv run python
EXCLUDE = llm_sdk,.venv,.mypy_cache,__pycache__

.PHONY: install run debug clean lint lint-strict

install:
	uv sync

run:
	$(PYTHON) -m src

debug:
	$(PYTHON) -m pdb -m src

clean:
	find . -type d -name __pycache__ -not -path "./.venv/*" -prune -exec rm -rf {} +
	rm -rf .mypy_cache .pytest_cache

lint:
	-uv run flake8 . --exclude=$(EXCLUDE)
	uv run mypy . --exclude '^(llm_sdk|\.venv)/' --ignore-missing-imports