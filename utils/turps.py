#!/usr/bin/env python3
"""Loads a DAS excel spreadsheet and linearly interpolates between each point to make a single table."""
import argparse
import pandas as pd
from typing import List, Tuple, TypeVar, Callable
import numpy as np

T = TypeVar('T')

def load(excel:pd.ExcelFile, excluded_sheets:List[str]=[]) -> List[pd.DataFrame]:
    """Returns a list of sheets to merge.

    Args:
        excel (pd.ExcelFile): The spreadsheet to load.
        excluded_sheets (List[str], optional): Sheet names to exclude. Defaults to [].

    Returns:
        List[pd.DataFrame]: A list of all sheets.
    """
    sheets = []
    for i in excel.sheet_names:
        if i not in excluded_sheets:
            # Add an interpolator for this sheet.
            sheets.append(excel.parse(i))

    return sheets

def time_getter(aggregator:Callable[[float], T]) -> Callable[[pd.DataFrame], T]:
    """Returns a function that can aggregate the time field in a specific way."""
    return lambda frame: aggregator(frame["unix_time"])

max_time = time_getter(max)
min_time = time_getter(min)

def get_after(frame:pd.DataFrame, time:float) -> pd.Series:
    """Gets the first row on or after a specific time.

    Args:
        frame (pd.DataFrame): The dataframe to search in.
        time (float): The time to search for.

    Returns:
        pd.Series: The first row on or after the time.
    """
    time_comparisons = frame["unix_time"] >= time
    after = frame[time_comparisons]
    if len(after):
        return after.iloc[0]
    else:
        return None

def get_before(frame:pd.DataFrame, time:float) -> pd.Series:
    """Gets the first row on or before a specific time.

    Args:
        frame (pd.DataFrame): The dataframe to search in.
        time (float): The time to search for.

    Returns:
        pd.Series: The first row on or before the time.
    """
    time_comparisons = frame["unix_time"] <= time
    before = frame[time_comparisons]
    if len(before):
        return before.iloc[-1]
    else:
        return None

def interpolate(t1:float, t2:float, n1:float, n2:float, time_target:float) -> float:
    """Linearly interpolates between two points.

    Args:
        t1 (float): The time of the first point.
        t2 (float): The time of the second point.
        n1 (float): The value of the first point.
        n2 (float): The value of the second point.
        time_target (float): The time to estimate the value of.

    Returns:
        float: The linearly interpolated value at time_target.
    """
    # y - y1 = m(x - x1)
    # m = (y2 - y1) / (x2 - x1)
    m = (n1 - n2) / (t1 - t2)
    return m*(time_target - t1) + n1

def merge(sheets:List[pd.DataFrame], t_step=1) -> pd.DataFrame:
    """Merges all sheets into one by interpolating between them and resampling.

    Args:
        sheets (List[pd.DataFrame]): The sheets to merge.

    Returns:
        pd.DataFrame: The merged and interpolated data frame.
    """
    min_t = min([min_time(i) for i in sheets])
    max_t = max([max_time(i) for i in sheets])

    rows = []
    for time in np.arange(min_t, max_t, t_step):
        time_rows = []
        for sheet in sheets:
            # Get the row before the current time and after for the current sheet.
            before = get_before(sheet, time)
            after = get_after(sheet, time)

            if after and not before:
                # At the very beginning of this sheet.
                time_rows.append(after)
            elif before and not after:
                # At the very end of this sheet.
                time_rows.append(before)
            else:
                # There are two rows. Linearly interpolate.
                # time_rows.app
                pass

            rows.append(time_rows)



if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        "turps", "linearly interpolates between DAS log files and merges all tabes"
    )
    parser.add_argument(
        "-i", "--input",
        help="The input file",
        type=str,
        required=True
    )
    args = parser.parse_args()
    sheets = load(pd.ExcelFile(args.input))
    merge(sheets)