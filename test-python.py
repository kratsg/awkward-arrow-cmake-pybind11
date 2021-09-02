import json

import awkward as ak
import numpy as np
import pyarrow as pa

import build.babel
import build.example


def test_producer():
    complicated_data = [1.1, 2.2, 3.3, [1, 2, 3], [], [4, 5], {"x": 12.3, "y": "wow"}]

    assert ak.to_list(build.example.producer()) == complicated_data

    assert (
        json.loads(build.example.consumer(ak.Array(complicated_data).layout))
        == complicated_data
    )


def test_sum():
    data = [1, 2, 3.1]
    x = pa.array(data)
    assert build.babel.sum(x) == np.sum(np.array(data))
    assert build.babel.sum(x[1:]) == np.sum(np.array(x[1:]))


def test_awkward_roundtrip():
    in_arr = ak.Array([1.0, 2.0, 3.0])
    in_arrow = ak.to_arrow(in_arr)
    out_arrow = build.babel.times_two(in_arrow)
    out_arr = ak.from_arrow(out_arrow)
    assert ak.to_numpy(out_arr).tolist() == [3.1, 4.2, 5.3]


def test_struct():
    s = build.babel.handle_struct()
    a = ak.from_arrow(s)
    assert str(ak.type(a)) == '3 * {"pt": ?float64, "eta": ?float64}'


if __name__ == "__main__":
    test_sum()
    test_awkward_roundtrip()
    test_struct()
    test_producer()
