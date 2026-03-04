# Minimaphud

Utility helpers to avoid "hallucinating" functions when calling across classes.

## Safe dispatcher

`src/safe_dispatcher.py` provides `SafeDispatcher`, a small registry that only calls functions that actually exist on registered class instances. It raises clear errors when a service is missing, when the requested function does not exist, or when the attribute is not callable.

```python
from src.safe_dispatcher import SafeDispatcher


class GreetingService:
    def greet(self, name: str) -> str:
        return f"Hello, {name}"


dispatcher = SafeDispatcher()
dispatcher.register("greeting", GreetingService())
message = dispatcher.call("greeting", "greet", "Ada")
```

## Tests

Run the unit tests from the repository root:

```bash
python -m unittest
```
