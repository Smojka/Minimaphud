"""Utilities to safely call functions on registered class instances without guessing their members."""

from typing import Any, Callable, Dict


class SafeDispatcher:
    """Registers class instances and calls their functions only when they actually exist."""

    def __init__(self) -> None:
        self._services: Dict[str, Any] = {}

    def register(self, name: str, instance: Any) -> None:
        if not name:
            raise ValueError("Service name must be non-empty.")
        self._services[name] = instance

    def call(self, service_name: str, fn_name: str, *args: Any, **kwargs: Any) -> Any:
        if service_name not in self._services:
            raise LookupError(f"Service '{service_name}' is not registered.")

        service = self._services[service_name]
        if not hasattr(service, fn_name):
            raise AttributeError(f"Service '{service_name}' does not have function '{fn_name}'.")

        fn: Callable[..., Any] = getattr(service, fn_name)
        if not callable(fn):
            raise TypeError(f"Attribute '{fn_name}' on service '{service_name}' is not callable.")

        return fn(*args, **kwargs)
