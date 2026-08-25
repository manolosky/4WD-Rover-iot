#!/usr/bin/env python3
# ============================================================
# test_drive.py — Systematic mobility test (Phase 1)
#
# Sends command sequences to the rover's /cmd endpoint to verify
# each movement separately: forward, reverse, arc turns and
# rotation in place.
#
# Usage:
#   1. Connect the PC to the "Rover-4WD" WiFi network
#   2. Rover lifted (wheels in the air) for the first pass
#   3. python test_drive.py [host] [--speed 40]
#
# The firmware has a failsafe: if commands stop arriving for
# 600ms, the motors stop on their own.
# ============================================================
import sys
import time
import urllib.request
import urllib.error

DEFAULT_HOST = "192.168.4.1"
SEND_INTERVAL_S = 0.1   # 10Hz, below the 600ms CMD_TIMEOUT_MS
MOVE_DURATION_S = 2.0

# (name, x, y, what to observe)
SEQUENCE = [
    ("FORWARD",         0,  None, "All 4 wheels spin FORWARD"),
    ("REVERSE",         0,  "-",  "All 4 wheels spin BACKWARD"),
    ("LEFT TURN",       "-", None, "Right wheels faster than left ones (arc to the left)"),
    ("RIGHT TURN",      "+", None, "Left wheels faster than right ones (arc to the right)"),
    ("SPIN LEFT (in place)",  "-100", 0, "Left wheels BACKWARD, right wheels FORWARD"),
    ("SPIN RIGHT (in place)", "+100", 0, "Left wheels FORWARD, right wheels BACKWARD"),
]


def send_cmd(host: str, x: int, y: int) -> bool:
    url = f"http://{host}/cmd?x={x}&y={y}"
    try:
        with urllib.request.urlopen(url, timeout=1) as resp:
            return 200 == resp.status
    except (urllib.error.URLError, OSError):
        return False


def hold_movement(host: str, x: int, y: int, duration_s: float) -> None:
    # Re-send the command periodically to keep the failsafe alive
    end = time.time() + duration_s
    while time.time() < end:
        send_cmd(host, x, y)
        time.sleep(SEND_INTERVAL_S)
    send_cmd(host, 0, 0)  # explicit stop when done


def resolve(value, speed: int) -> int:
    # Convert SEQUENCE tokens to concrete -100..100 values
    if None is value:
        return speed
    if 0 == value:
        return 0
    if "-" == value:
        return -speed
    if "+" == value:
        return speed
    return int(value)


def main() -> None:
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    host = args[0] if args else DEFAULT_HOST
    speed = 40
    for arg in sys.argv[1:]:
        if arg.startswith("--speed"):
            speed = int(arg.split("=")[1] if "=" in arg else sys.argv[sys.argv.index(arg) + 1])
    speed = max(10, min(100, speed))

    print(f"Rover: http://{host}  |  Test speed: {speed}%")
    print("Checking connection...")
    if not send_cmd(host, 0, 0):
        print("ERROR: no response from the rover. Are you connected to the 'Rover-4WD' WiFi?")
        sys.exit(1)
    print("Connected. Rover lifted (wheels in the air) recommended for the first pass.\n")

    for name, x_tok, y_tok in [(s[0], s[1], s[2]) for s in SEQUENCE]:
        expected = next(s[3] for s in SEQUENCE if s[0] == name)
        x = resolve(x_tok, speed)
        y = resolve(y_tok, speed)
        input(f">> {name}  (x={x}, y={y})\n   Expected: {expected}\n   [Enter] to run for {MOVE_DURATION_S:.0f}s... ")
        hold_movement(host, x, y, MOVE_DURATION_S)
        print("   Stopped.\n")

    print("Sequence complete. Failsafe test: move the web joystick and turn off")
    print("the phone's WiFi: the rover must stop in under 1 second.")


if "__main__" == __name__:
    main()
