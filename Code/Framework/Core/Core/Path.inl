#pragma once

template<typename... TStringViews>
str::Path::Path(const str::EPath type, const TStringViews& ...args)
{
	*this = GetPath(type);
	(this->AppendDir(args), ...);
}

template<typename... TStringViews>
str::Path::Path(const str::Path& value, const TStringViews& ...args)
{
	*this = value;
	(this->AppendDir(args),...);
}