import unittest

from src.safe_dispatcher import SafeDispatcher


class GreetingService:
    def greet(self, name: str) -> str:
        return f"Hello, {name}"


class NonCallableService:
    value = 42


class SafeDispatcherTests(unittest.TestCase):
    def setUp(self) -> None:
        self.dispatcher = SafeDispatcher()

    def test_calls_existing_function(self) -> None:
        self.dispatcher.register("greeting", GreetingService())

        result = self.dispatcher.call("greeting", "greet", "Ada")

        self.assertEqual("Hello, Ada", result)

    def test_missing_service_raises(self) -> None:
        with self.assertRaisesRegex(LookupError, "not registered"):
            self.dispatcher.call("unknown", "greet")

    def test_missing_function_raises(self) -> None:
        self.dispatcher.register("greeting", GreetingService())

        with self.assertRaisesRegex(AttributeError, "does not have function 'farewell'"):
            self.dispatcher.call("greeting", "farewell")

    def test_non_callable_attribute_raises(self) -> None:
        self.dispatcher.register("values", NonCallableService())

        with self.assertRaisesRegex(TypeError, "not callable"):
            self.dispatcher.call("values", "value")

    def test_rejects_empty_service_name(self) -> None:
        with self.assertRaisesRegex(ValueError, "non-empty"):
            self.dispatcher.register("", GreetingService())


if __name__ == "__main__":
    unittest.main()
