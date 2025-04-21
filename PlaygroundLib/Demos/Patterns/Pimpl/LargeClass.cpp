#include "LargeClass.hpp"
#include "LargeClassImpl.hpp"

LargeClass::LargeClass() : mImpl(std::make_unique<LargeClassImpl>()) {}

std::string LargeClass::GetString()
{
    return mImpl->GetString();
}
int LargeClass::GetInt()
{
    return mImpl->GetInt();
}
double LargeClass::GetDouble()
{
    return mImpl->GetDouble();
}

LargeClass::LargeClass(const LargeClass &other):mImpl(std::make_unique<LargeClassImpl>(*other.mImpl)){}

LargeClass &LargeClass::operator=(const LargeClass &other)
{
    *mImpl = *other.mImpl;
    return *this;
}

LargeClass::LargeClass(LargeClass &&other) = default;

LargeClass &LargeClass::operator=(LargeClass &&other) = default;

LargeClass::~LargeClass() = default;