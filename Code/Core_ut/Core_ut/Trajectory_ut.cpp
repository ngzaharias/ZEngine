#include <Catch2/catch.hpp>

#include "Core/Trajectory.h"
#include "Core/Vector.h"

TEST_CASE("path::Trajectory")
{
	SECTION("Default")
	{
		path::Trajectory trajectory;
		CHECK(trajectory.GetLength() == 0.f);
		CHECK(trajectory.AtDistance(0.f).x == Approx(0.f));
		CHECK(trajectory.AtDistance(1.f).x == Approx(0.f));
		CHECK(trajectory.AtPercentage(0.f).x == Approx(0.f));
		CHECK(trajectory.AtPercentage(1.f).x == Approx(0.f));
	}

	SECTION("Bezier3")
	{
		{
			INFO("Simple");
			Bezier3 settings;
			settings.m_PointA = Vector3f(0.f, 0.f, 0.f);
			settings.m_Control = Vector3f(1.5f, 0.f, 0.f);
			settings.m_PointB = Vector3f(3.f, 0.f, 0.f);

			path::Trajectory trajectory(settings);
			CHECK(trajectory.GetLength() == Approx(3.f));
			CHECK(trajectory.AtDistance(0.f).x == Approx(0.f));
			CHECK(trajectory.AtDistance(1.f).x == Approx(1.f));
			CHECK(trajectory.AtDistance(2.f).x == Approx(2.f));
			CHECK(trajectory.AtDistance(3.f).x == Approx(3.f));
			CHECK(trajectory.AtPercentage(0.0f).x == Approx(0.0f));
			CHECK(trajectory.AtPercentage(0.5f).x == Approx(1.5f));
			CHECK(trajectory.AtPercentage(1.0f).x == Approx(3.0f));
		}
	}

	SECTION("Bezier4")
	{
		{
			INFO("Simple");
			Bezier4 settings;
			settings.m_PointA = Vector3f(0.f, 0.f, 0.f);
			settings.m_ControlA = Vector3f(1.f, 0.f, 0.f);
			settings.m_ControlB = Vector3f(2.f, 0.f, 0.f);
			settings.m_PointB = Vector3f(3.f, 0.f, 0.f);

			path::Trajectory trajectory(settings);
			CHECK(trajectory.GetLength() == Approx(3.f));
			CHECK(trajectory.AtDistance(0.f).x == Approx(0.f));
			CHECK(trajectory.AtDistance(1.f).x == Approx(1.f));
			CHECK(trajectory.AtDistance(2.f).x == Approx(2.f));
			CHECK(trajectory.AtDistance(3.f).x == Approx(3.f));
			CHECK(trajectory.AtPercentage(0.0f).x == Approx(0.0f));
			CHECK(trajectory.AtPercentage(0.5f).x == Approx(1.5f));
			CHECK(trajectory.AtPercentage(1.0f).x == Approx(3.0f));
		}
	}

	SECTION("Points")
	{
		{
			INFO("Even distribution.");
			path::Points settings;
			settings.m_Distances.Append(0.f);
			settings.m_Distances.Append(1.f);
			settings.m_Distances.Append(2.f);
			settings.m_Distances.Append(3.f);
			settings.m_Positions.Append(Vector3f(0.f, 0.f, 0.f));
			settings.m_Positions.Append(Vector3f(1.f, 0.f, 0.f));
			settings.m_Positions.Append(Vector3f(2.f, 0.f, 0.f));
			settings.m_Positions.Append(Vector3f(3.f, 0.f, 0.f));

			path::Trajectory trajectory(settings);
			CHECK(trajectory.GetLength() == Approx(3.f));
			CHECK(trajectory.AtDistance(0.f).x == Approx(0.f));
			CHECK(trajectory.AtDistance(1.f).x == Approx(1.f));
			CHECK(trajectory.AtDistance(2.f).x == Approx(2.f));
			CHECK(trajectory.AtDistance(3.f).x == Approx(3.f));
			CHECK(trajectory.AtPercentage(0.0f).x == Approx(0.0f));
			CHECK(trajectory.AtPercentage(0.5f).x == Approx(1.5f));
			CHECK(trajectory.AtPercentage(1.0f).x == Approx(3.0f));
		}

		{
			INFO("Un-even distribution.");
			path::Points settings;
			settings.m_Distances.Append(0.0f);
			settings.m_Distances.Append(0.1f);
			settings.m_Distances.Append(0.2f);
			settings.m_Distances.Append(0.3f);
			settings.m_Distances.Append(3.0f);
			settings.m_Positions.Append(Vector3f(0.0f, 0.f, 0.f));
			settings.m_Positions.Append(Vector3f(0.1f, 0.f, 0.f));
			settings.m_Positions.Append(Vector3f(0.2f, 0.f, 0.f));
			settings.m_Positions.Append(Vector3f(0.3f, 0.f, 0.f));
			settings.m_Positions.Append(Vector3f(3.0f, 0.f, 0.f));

			path::Trajectory trajectory(settings);
			CHECK(trajectory.GetLength() == Approx(3.f));
			CHECK(trajectory.AtDistance(0.f).x == Approx(0.f));
			CHECK(trajectory.AtDistance(1.f).x == Approx(1.f));
			CHECK(trajectory.AtDistance(2.f).x == Approx(2.f));
			CHECK(trajectory.AtDistance(3.f).x == Approx(3.f));
			CHECK(trajectory.AtPercentage(0.0f).x == Approx(0.0f));
			CHECK(trajectory.AtPercentage(0.5f).x == Approx(1.5f));
			CHECK(trajectory.AtPercentage(1.0f).x == Approx(3.0f));
		}
	}
}
