#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Python.h>

#include <iostream>

#include <arrow/python/pyarrow.h>
#include <arrow/array/builder_primitive.h>
#include <arrow/array.h>
#include <arrow/compute/function.h>

namespace py = pybind11;
using DoubleArray = arrow::DoubleArray;
using Array = arrow::Array;


double sum(py::handle& source) {
    PyObject* src = source.ptr();

    if(!arrow::py::is_array(src)){
        std::cout << "not an array" << std::endl;
        return false;
    }

    arrow::Result<std::shared_ptr<arrow::Array>> result = arrow::py::unwrap_array(src);
    if(!result.ok())
        return false;

    std::shared_ptr<arrow::DoubleArray> a = std::static_pointer_cast<arrow::DoubleArray>(result.ValueOrDie());

    double sum = 0;
    for(int i = 0; i < a->length(); i++) {
        sum += a->Value(i);
    }
    return sum;
}

py::handle times_two(py::handle& source) {
    PyObject* src = source.ptr();

    if(!arrow::py::is_array(src)){
        std::cout << "not an array" << std::endl;
    }

    arrow::Result<std::shared_ptr<arrow::Array>> result = arrow::py::unwrap_array(src);
    if(!result.ok()){
        std::cout << "not ok" << std::endl;

    }

    std::shared_ptr<arrow::DoubleArray> a = std::static_pointer_cast<arrow::DoubleArray>(result.ValueOrDie());
    std::cout << "a: " << a << std::endl;
    std::cout << "a: " << a->values()->data() << std::endl;
    std::cout << "a: " << a->ToString() << std::endl;


    arrow::DoubleBuilder builder;
    builder.Resize(3);

    std::vector<bool> validity = {true, true, true};
    std::vector<double> values = {3.1, 4.2 ,5.3};

    builder.AppendValues(values, validity);
    
    std::shared_ptr<arrow::Array> array;
    arrow::Status st = builder.Finish(&array);

    if(!st.ok()){
        std::cout << "not ok" << std::endl;
    }

    return arrow::py::wrap_array(array);
}

std::shared_ptr<arrow::Array> build(const std::vector<double>& values, const std::vector<bool>& validity){
    arrow::DoubleBuilder builder;
    builder.Resize(3);
    builder.AppendValues(values, validity);
    std::shared_ptr<arrow::Array> array;
    arrow::Status st = builder.Finish(&array);
    if(!st.ok()){
        std::cout << "not ok" << std::endl;
    }
    return array;
}

py::handle handle_struct() {
    auto pt = build({3.1, 4.2 ,5.3},{true, true, true});
    auto eta = build({0.1, 0.2 ,0.3},{true, true, true});

    std::vector<std::string> fields = {"pt","eta"};
    auto x = arrow::StructArray::Make(
        {pt,eta}, fields
    ).ValueOrDie();
    return arrow::py::wrap_array(x);
}

PYBIND11_MODULE(babel, m) {
  // Ensure dependencies are loaded.
  arrow::py::import_pyarrow();

  m.def("sum", &sum, py::call_guard<py::gil_scoped_release>());
  m.def("times_two", &times_two);
  m.def("handle_struct", &handle_struct);
}
