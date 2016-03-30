#include "printer.h"
#include "benchmark.h"

#include <fstream>
#include <string>

namespace geiger
{

namespace printer
{

struct csv : public printer_base
{
    csv(const std::string& filename, char delimiter = ';')
     : m_filename(filename),
       m_delimiter(delimiter)
    {
    }

    csv(const csv& c) =delete;

    csv(csv&& c)
    : m_filename(std::move(c.m_filename)),
      m_delimiter(c.m_delimiter)
    {
        c.m_ofile.close();
    }

    void on_start(const suite_base& s) override
    {
        m_ofile.open(m_filename.c_str());

        if (!m_ofile.is_open())
            throw std::runtime_error("geiger::printer::csv: unable to open file " + m_filename);

        m_ofile << "#Test" << m_delimiter << "Time";

        std::vector<int> papi_events = s.papi_events();
        for (auto it = papi_events.begin(); it != papi_events.end(); ++it)
        {
            m_ofile << m_delimiter << get_papi_event_name(*it);
        }
    }

    void on_test_complete(const std::string& name, const test_report& r) override
    {
        m_ofile << "\n" << name << m_delimiter << r.time_per_task().count();

        for (long long counter : r.papi_counters())
        {
            m_ofile << m_delimiter << counter;
        }
    }

private:
    std::ofstream m_ofile;

    std::string m_filename;
    char        m_delimiter;
};

}

}
