#include "Meter.h"

#include <algorithm>
#include <boost/lexical_cast.hpp>

const float Meter::DEFAULT_VALUE = 0.0;
const float Meter::LARGE_VALUE = static_cast<float>(2 << 15);
const float Meter::INVALID_VALUE = -LARGE_VALUE;

Meter::Meter() :
    m_current_value(DEFAULT_VALUE),
    m_initial_value(DEFAULT_VALUE)
{}

Meter::Meter(float current_value) :
    m_current_value(current_value),
    m_initial_value(DEFAULT_VALUE)
{}

Meter::Meter(float current_value, float initial_value) :
    m_current_value(current_value),
    m_initial_value(initial_value)
{}

float Meter::Current() const
{ return m_current_value; }

float Meter::Initial() const
{ return m_initial_value; }

std::string Meter::Dump() const
{ return "Cur: " + boost::lexical_cast<std::string>(m_current_value) + " Initial: " + boost::lexical_cast<std::string>(m_initial_value); }

void Meter::SetCurrent(float current_value)
{ m_current_value = current_value; }

void Meter::Set(float current_value, float initial_value) {
    m_current_value = current_value;
    m_initial_value = initial_value;
}

void Meter::ResetCurrent()
{ m_current_value = DEFAULT_VALUE; } // initial unchanged

void Meter::Reset() {
    m_current_value = DEFAULT_VALUE;
    m_initial_value = DEFAULT_VALUE;
}

void Meter::AddToCurrent(float adjustment)
{ m_current_value += adjustment; }

void Meter::ClampCurrentToRange(float min/* = DEFAULT_VALUE*/, float max/* = LARGE_VALUE*/)
{ m_current_value = std::max(std::min(m_current_value, max), min); }

void Meter::BackPropegate()
{ m_initial_value = m_current_value; }
