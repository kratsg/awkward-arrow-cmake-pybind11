import json
import build.example
import awkward as ak

def test_producer():
    complicated_data = [1.1, 2.2, 3.3, [1, 2, 3], [], [4, 5], {"x": 12.3, "y": "wow"}]

    assert ak.to_list(build.dependent.producer()) == complicated_data

    assert (
        json.loads(build.dependent.consumer(ak.Array(complicated_data).layout))
        == complicated_data
    )

import pyarrow as pa
import numpy as np

import build.babel

def test_sum():
    data = [1,2,3.1]
    x = pa.array(data)
    assert build.babel.sum(x) == np.sum(np.array(data))
    assert build.babel.sum(x[1:]) == np.sum(np.array(x[1:]))


if __name__ == '__main__':
    test_sum()