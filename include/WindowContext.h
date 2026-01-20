#pragma once

#include <SFML/Graphics.hpp>

#include <functional>

struct WindowContext
{
public:
	using ResizeCallback = std::function<void(const sf::Vector2u&)>;

	explicit WindowContext(sf::VideoMode mode, const std::string title)
	  :	window_(sf::RenderWindow(mode, title)) 
	{
		// Load font
		if (!font_.openFromFile("res/OpenSans.ttf"))
			throw std::runtime_error("ERR: Font could not be loaded\n");
	}

	inline sf::RenderWindow& GetWindow() { return window_; }

	inline sf::Vector2u GetWindowSize2u() const { return window_.getSize(); }
	inline sf::Vector2f GetWindowSize() const { return sf::Vector2f(window_.getSize()); }
	inline sf::Vector2f GetWindowCenter() const { return sf::Vector2f(window_.getSize().x / 2.f, window_.getSize().y / 2.f); }

	inline void PushCallback(ResizeCallback callback) { listeners_.push_back(callback); }
	inline void NotifyResize() { for (const auto& listener : listeners_) listener(window_.getSize()); }
	
private:
	std::vector<ResizeCallback> listeners_;
	sf::RenderWindow window_;
	sf::Font font_;
};