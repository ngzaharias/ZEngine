#include <Catch2/catch.hpp>

#include "Engine/EnginePCH.h"
#include "Engine/SinkCollection.h"

#include <entt/signal/sigh.hpp>

namespace
{
	class Helper
	{
	public:
		void FunctionA() { m_ValueA = true; }
		void FunctionB() { m_ValueB = true; }

		bool m_ValueA = false;
		bool m_ValueB = false;
	};
}

TEST_CASE("SinkCollection")
{
	Helper helper;
	entt::sigh<void()> signalA;
	entt::sigh<void()> signalB;

	SinkCollection connections;
	SECTION("Single Connection")
	{
		connections = {	entt::sink(signalA).connect<&Helper::FunctionA>(&helper) };

		signalA.publish();
		signalB.publish();
		CHECK(helper.m_ValueA);
		CHECK(!helper.m_ValueB);
	}

	SECTION("Multiple Connections")
	{
		connections = {
			entt::sink(signalA).connect<&Helper::FunctionA>(&helper),
			entt::sink(signalB).connect<&Helper::FunctionB>(&helper) };

		signalA.publish();
		CHECK(helper.m_ValueA);
		CHECK(!helper.m_ValueB);

		signalB.publish();
		CHECK(helper.m_ValueA);
		CHECK(helper.m_ValueB);
	}

	SECTION("Disconnect")
	{
		connections = {
			entt::sink(signalA).connect<&Helper::FunctionA>(&helper),
			entt::sink(signalB).connect<&Helper::FunctionB>(&helper) };
		connections.Disconnect();

		signalA.publish();
		signalB.publish();
		CHECK(!helper.m_ValueA);
		CHECK(!helper.m_ValueB);
	}

	SECTION("Reconnect")
	{
		connections = { entt::sink(signalA).connect<&Helper::FunctionA>(&helper) };
		connections.Disconnect();
		connections = { entt::sink(signalB).connect<&Helper::FunctionB>(&helper) };

		signalA.publish();
		signalB.publish();
		CHECK(!helper.m_ValueA);
		CHECK(helper.m_ValueB);
	}
}