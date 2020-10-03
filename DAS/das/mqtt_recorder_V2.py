import argparse
import os
import time
import sys
from das.utils import logger

parser = argparse.ArgumentParser(
    description="MQTT logger",
    add_help=True,
    formatter_class=argparse.ArgumentDefaultsHelpFormatter,
)

parser.add_argument(
    "topics",
    nargs="*",
    action="store",
    default=["#"],
    help="""Verbose logging output""",
)

parser.add_argument(
    "--host",
    action="store",
    type=str,
    default="localhost",
    help="""Address of the MQTT broker""",
)

parser.add_argument(
    "-v",
    "--verbose",
    action="store_true",
    default=False,
    help="""Verbose logging output""",
)

parser.add_argument(
    "-t",
    "--time",
    action="store",
    type=float,
    default=float("Inf"),
    help="""Length of time to record data
    (duration). If nothing is selected it will continuously record data.""",
)

if __name__ == "__main__":
    CURRENT_FILEPATH = os.path.dirname(__file__)
    CSV_FILEPATH = os.path.join(CURRENT_FILEPATH, "csv_data")

    # Read command line arguments
    args = parser.parse_args()

    # Make logger object and initiate logging
    main_recorder = logger.Recorder(
        CSV_FILEPATH, topics=args.topics, broker_address=args.host, verbose=args.verbose
    )

    # Start the logger
    main_recorder.start()

    # Logger can run forever or for a specific time
    try:
        if args.time != float("Inf"):
            time.sleep(args.time)
        else:
            while True:
                pass

    except (KeyboardInterrupt, Exception):
        pass

    finally:
        # Graceful exit that nicely quits the recorder to ensure the data is saved
        main_recorder.stop()
        sys.exit()