#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "FakeCamera.h"
#include "HdrCombiner.h"

namespace py = pybind11;

PYBIND11_MODULE(hdr, m) {
    m.doc() = "HDR Image Combiner Python Bindings";

    py::class_<FakeCamera>(m, "FakeCamera")
        .def(py::init<int, int>(), py::arg("width"), py::arg("height"))
        .def("capture", &FakeCamera::capture, py::arg("exposure_ms"));

    py::class_<HdrCombiner>(m, "HdrCombiner")
        .def(py::init<int, int>(), py::arg("width"), py::arg("height"))
        .def("combine", &HdrCombiner::combine,
            py::arg("img1"), py::arg("img2"), py::arg("img3"),
            py::arg("e1"), py::arg("e2"), py::arg("e3"));
}