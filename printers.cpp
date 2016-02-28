#include "printers.h"
#include "benchmark.h"
#include "papi.h"

#include <string>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <locale>

namespace geiger
{

namespace printers
{

void console::on_start(const suite_base& s)
{
    std::vector<std::reference_wrapper<const std::string>> names = s.test_names();

    auto it = std::max_element(names.begin(), names.end(), [](const std::string& s1, const std::string& s2)
                               {
                                   return s1.size() < s2.size();
                               });

    m_first_col_width = it->get().size();

    int width = fprintf(stdout, "%-*s %10s", m_first_col_width, "Test", "Time (ns)");

    std::vector<int> papi_events = s.papi_events();
    for (int event : papi_events)
    {
        std::string event_name = get_papi_event_name(event);
        width += fprintf(stdout, " %12s", event_name.c_str());
    }

    std::cout << "\n" << std::string(width, '-') << std::endl;
}

void console::on_test_complete(const std::string& name, const test_report& r)
{
    fprintf(stdout, "%-*s %10ld", m_first_col_width, name.c_str(), r.time_per_task().count());

    for (long long counter : r.papi_counters())
        fprintf(stdout, " %12lld", counter);

    std::cout << std::endl;
}
}
}
