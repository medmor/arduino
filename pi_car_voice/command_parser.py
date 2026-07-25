"""Parse recognized speech text into robot actions."""

from config import COMMANDS


def parse_command(text: str) -> str | None:
    """Return the first matching action for the given text, or None."""
    text = text.lower().strip()
    for action, phrases in COMMANDS.items():
        for phrase in phrases:
            if phrase in text:
                return action
    return None
