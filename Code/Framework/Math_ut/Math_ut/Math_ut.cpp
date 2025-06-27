#include "MathPCH.h"

#include "Math/Math.h"

TEST_CASE("math::Ceiling")
{
	// positives
	CHECK(math::Ceiling(+3.00f) == +3.00f);
	CHECK(math::Ceiling(+2.90f) == +3.00f);
	CHECK(math::Ceiling(+2.51f) == +3.00f);
	CHECK(math::Ceiling(+2.50f) == +3.00f);
	CHECK(math::Ceiling(+2.49f) == +3.00f);
	CHECK(math::Ceiling(+2.10f) == +3.00f);
	CHECK(math::Ceiling(+2.00f) == +2.00f);

	// negatives
	CHECK(math::Ceiling(-3.00f) == -3.00f);
	CHECK(math::Ceiling(-2.90f) == -2.00f);
	CHECK(math::Ceiling(-2.51f) == -2.00f);
	CHECK(math::Ceiling(-2.50f) == -2.00f);
	CHECK(math::Ceiling(-2.49f) == -2.00f);
	CHECK(math::Ceiling(-2.10f) == -2.00f);
	CHECK(math::Ceiling(-2.00f) == -2.00f);

	// positive, positive multiplier
	CHECK(math::Ceiling(+3.00f, +1.0f) == +3.00f);
	CHECK(math::Ceiling(+2.90f, +1.0f) == +3.00f);
	CHECK(math::Ceiling(+2.51f, +1.0f) == +3.00f);
	CHECK(math::Ceiling(+2.50f, +1.0f) == +3.00f);
	CHECK(math::Ceiling(+2.49f, +1.0f) == +3.00f);
	CHECK(math::Ceiling(+2.10f, +1.0f) == +3.00f);
	CHECK(math::Ceiling(+2.00f, +1.0f) == +2.00f);

	CHECK(math::Ceiling(+3.00f, +0.5f) == +3.00f);
	CHECK(math::Ceiling(+2.90f, +0.5f) == +3.00f);
	CHECK(math::Ceiling(+2.51f, +0.5f) == +3.00f);
	CHECK(math::Ceiling(+2.50f, +0.5f) == +2.50f);
	CHECK(math::Ceiling(+2.49f, +0.5f) == +2.50f);
	CHECK(math::Ceiling(+2.10f, +0.5f) == +2.50f);
	CHECK(math::Ceiling(+2.00f, +0.5f) == +2.00f);

	// positive, negative multiplier
	CHECK(math::Ceiling(+3.00f, -1.0f) == +3.00f);
	CHECK(math::Ceiling(+2.90f, -1.0f) == +2.00f);
	CHECK(math::Ceiling(+2.51f, -1.0f) == +2.00f);
	CHECK(math::Ceiling(+2.50f, -1.0f) == +2.00f);
	CHECK(math::Ceiling(+2.49f, -1.0f) == +2.00f);
	CHECK(math::Ceiling(+2.10f, -1.0f) == +2.00f);
	CHECK(math::Ceiling(+2.00f, -1.0f) == +2.00f);

	CHECK(math::Ceiling(+3.00f, -0.5f) == +3.00f);
	CHECK(math::Ceiling(+2.90f, -0.5f) == +2.50f);
	CHECK(math::Ceiling(+2.51f, -0.5f) == +2.50f);
	CHECK(math::Ceiling(+2.50f, -0.5f) == +2.50f);
	CHECK(math::Ceiling(+2.49f, -0.5f) == +2.00f);
	CHECK(math::Ceiling(+2.10f, -0.5f) == +2.00f);
	CHECK(math::Ceiling(+2.00f, -0.5f) == +2.00f);

	// negative, positive multiplier
	CHECK(math::Ceiling(-3.00f, +1.0f) == -3.00f);
	CHECK(math::Ceiling(-2.90f, +1.0f) == -2.00f);
	CHECK(math::Ceiling(-2.51f, +1.0f) == -2.00f);
	CHECK(math::Ceiling(-2.50f, +1.0f) == -2.00f);
	CHECK(math::Ceiling(-2.49f, +1.0f) == -2.00f);
	CHECK(math::Ceiling(-2.10f, +1.0f) == -2.00f);
	CHECK(math::Ceiling(-2.00f, +1.0f) == -2.00f);

	CHECK(math::Ceiling(-3.00f, +0.5f) == -3.00f);
	CHECK(math::Ceiling(-2.90f, +0.5f) == -2.50f);
	CHECK(math::Ceiling(-2.51f, +0.5f) == -2.50f);
	CHECK(math::Ceiling(-2.50f, +0.5f) == -2.50f);
	CHECK(math::Ceiling(-2.49f, +0.5f) == -2.00f);
	CHECK(math::Ceiling(-2.10f, +0.5f) == -2.00f);
	CHECK(math::Ceiling(-2.00f, +0.5f) == -2.00f);

	// negative, negative multiplier
	CHECK(math::Ceiling(-3.00f, -1.0f) == -3.00f);
	CHECK(math::Ceiling(-2.90f, -1.0f) == -3.00f);
	CHECK(math::Ceiling(-2.51f, -1.0f) == -3.00f);
	CHECK(math::Ceiling(-2.50f, -1.0f) == -3.00f);
	CHECK(math::Ceiling(-2.49f, -1.0f) == -3.00f);
	CHECK(math::Ceiling(-2.10f, -1.0f) == -3.00f);
	CHECK(math::Ceiling(-2.00f, -1.0f) == -2.00f);

	CHECK(math::Ceiling(-3.00f, -0.5f) == -3.00f);
	CHECK(math::Ceiling(-2.90f, -0.5f) == -3.00f);
	CHECK(math::Ceiling(-2.51f, -0.5f) == -3.00f);
	CHECK(math::Ceiling(-2.50f, -0.5f) == -2.50f);
	CHECK(math::Ceiling(-2.49f, -0.5f) == -2.50f);
	CHECK(math::Ceiling(-2.10f, -0.5f) == -2.50f);
	CHECK(math::Ceiling(-2.00f, -0.5f) == -2.00f);
}

TEST_CASE("math::Floor")
{
	// positives
	CHECK(math::Floor(+3.00f) == +3.00f);
	CHECK(math::Floor(+2.90f) == +2.00f);
	CHECK(math::Floor(+2.51f) == +2.00f);
	CHECK(math::Floor(+2.50f) == +2.00f);
	CHECK(math::Floor(+2.49f) == +2.00f);
	CHECK(math::Floor(+2.10f) == +2.00f);
	CHECK(math::Floor(+2.00f) == +2.00f);

	// negatives
	CHECK(math::Floor(-3.00f) == -3.00f);
	CHECK(math::Floor(-2.90f) == -3.00f);
	CHECK(math::Floor(-2.51f) == -3.00f);
	CHECK(math::Floor(-2.50f) == -3.00f);
	CHECK(math::Floor(-2.49f) == -3.00f);
	CHECK(math::Floor(-2.10f) == -3.00f);
	CHECK(math::Floor(-2.00f) == -2.00f);

	// positive, positive multiplier
	CHECK(math::Floor(+3.00f, +1.0f) == +3.00f);
	CHECK(math::Floor(+2.90f, +1.0f) == +2.00f);
	CHECK(math::Floor(+2.51f, +1.0f) == +2.00f);
	CHECK(math::Floor(+2.50f, +1.0f) == +2.00f);
	CHECK(math::Floor(+2.49f, +1.0f) == +2.00f);
	CHECK(math::Floor(+2.10f, +1.0f) == +2.00f);
	CHECK(math::Floor(+2.00f, +1.0f) == +2.00f);

	CHECK(math::Floor(+3.00f, +0.5f) == +3.00f);
	CHECK(math::Floor(+2.90f, +0.5f) == +2.50f);
	CHECK(math::Floor(+2.51f, +0.5f) == +2.50f);
	CHECK(math::Floor(+2.50f, +0.5f) == +2.50f);
	CHECK(math::Floor(+2.49f, +0.5f) == +2.00f);
	CHECK(math::Floor(+2.10f, +0.5f) == +2.00f);
	CHECK(math::Floor(+2.00f, +0.5f) == +2.00f);

	// positive, negative multiplier
	CHECK(math::Floor(+3.00f, -1.0f) == +3.00f);
	CHECK(math::Floor(+2.90f, -1.0f) == +3.00f);
	CHECK(math::Floor(+2.51f, -1.0f) == +3.00f);
	CHECK(math::Floor(+2.50f, -1.0f) == +3.00f);
	CHECK(math::Floor(+2.49f, -1.0f) == +3.00f);
	CHECK(math::Floor(+2.10f, -1.0f) == +3.00f);
	CHECK(math::Floor(+2.00f, -1.0f) == +2.00f);

	CHECK(math::Floor(+3.00f, -0.5f) == +3.00f);
	CHECK(math::Floor(+2.90f, -0.5f) == +3.00f);
	CHECK(math::Floor(+2.51f, -0.5f) == +3.00f);
	CHECK(math::Floor(+2.50f, -0.5f) == +2.50f);
	CHECK(math::Floor(+2.49f, -0.5f) == +2.50f);
	CHECK(math::Floor(+2.10f, -0.5f) == +2.50f);
	CHECK(math::Floor(+2.00f, -0.5f) == +2.00f);

	// negative, positive multiplier
	CHECK(math::Floor(-3.00f, +1.0f) == -3.00f);
	CHECK(math::Floor(-2.90f, +1.0f) == -3.00f);
	CHECK(math::Floor(-2.51f, +1.0f) == -3.00f);
	CHECK(math::Floor(-2.50f, +1.0f) == -3.00f);
	CHECK(math::Floor(-2.49f, +1.0f) == -3.00f);
	CHECK(math::Floor(-2.10f, +1.0f) == -3.00f);
	CHECK(math::Floor(-2.00f, +1.0f) == -2.00f);

	CHECK(math::Floor(-3.00f, +0.5f) == -3.00f);
	CHECK(math::Floor(-2.90f, +0.5f) == -3.00f);
	CHECK(math::Floor(-2.51f, +0.5f) == -3.00f);
	CHECK(math::Floor(-2.50f, +0.5f) == -2.50f);
	CHECK(math::Floor(-2.49f, +0.5f) == -2.50f);
	CHECK(math::Floor(-2.10f, +0.5f) == -2.50f);
	CHECK(math::Floor(-2.00f, +0.5f) == -2.00f);

	// negative, negative multiplier
	CHECK(math::Floor(-3.00f, -1.0f) == -3.00f);
	CHECK(math::Floor(-2.90f, -1.0f) == -2.00f);
	CHECK(math::Floor(-2.51f, -1.0f) == -2.00f);
	CHECK(math::Floor(-2.50f, -1.0f) == -2.00f);
	CHECK(math::Floor(-2.49f, -1.0f) == -2.00f);
	CHECK(math::Floor(-2.10f, -1.0f) == -2.00f);
	CHECK(math::Floor(-2.00f, -1.0f) == -2.00f);

	CHECK(math::Floor(-3.00f, -0.5f) == -3.00f);
	CHECK(math::Floor(-2.90f, -0.5f) == -2.50f);
	CHECK(math::Floor(-2.51f, -0.5f) == -2.50f);
	CHECK(math::Floor(-2.50f, -0.5f) == -2.50f);
	CHECK(math::Floor(-2.49f, -0.5f) == -2.00f);
	CHECK(math::Floor(-2.10f, -0.5f) == -2.00f);
	CHECK(math::Floor(-2.00f, -0.5f) == -2.00f);
}

TEST_CASE("math::Round")
{
	// positives
	CHECK(math::Round(+3.00f) == +3.00f);
	CHECK(math::Round(+2.90f) == +3.00f);
	CHECK(math::Round(+2.51f) == +3.00f);
	CHECK(math::Round(+2.50f) == +3.00f);
	CHECK(math::Round(+2.49f) == +2.00f);
	CHECK(math::Round(+2.10f) == +2.00f);
	CHECK(math::Round(+2.00f) == +2.00f);

	// negatives
	CHECK(math::Round(-3.00f) == -3.00f);
	CHECK(math::Round(-2.90f) == -3.00f);
	CHECK(math::Round(-2.51f) == -3.00f);
	CHECK(math::Round(-2.50f) == -3.00f);
	CHECK(math::Round(-2.49f) == -2.00f);
	CHECK(math::Round(-2.10f) == -2.00f);
	CHECK(math::Round(-2.00f) == -2.00f);

	// positive, positive multiplier
	CHECK(math::Round(+3.00f, +1.0f) == +3.00f);
	CHECK(math::Round(+2.90f, +1.0f) == +3.00f);
	CHECK(math::Round(+2.51f, +1.0f) == +3.00f);
	CHECK(math::Round(+2.50f, +1.0f) == +3.00f);
	CHECK(math::Round(+2.49f, +1.0f) == +2.00f);
	CHECK(math::Round(+2.10f, +1.0f) == +2.00f);
	CHECK(math::Round(+2.00f, +1.0f) == +2.00f);

	CHECK(math::Round(+3.00f, +0.5f) == +3.00f);
	CHECK(math::Round(+2.90f, +0.5f) == +3.00f);
	CHECK(math::Round(+2.51f, +0.5f) == +2.50f);
	CHECK(math::Round(+2.50f, +0.5f) == +2.50f);
	CHECK(math::Round(+2.49f, +0.5f) == +2.50f);
	CHECK(math::Round(+2.10f, +0.5f) == +2.00f);
	CHECK(math::Round(+2.00f, +0.5f) == +2.00f);

	// positive, negative multiplier
	CHECK(math::Round(+3.00f, -1.0f) == +3.00f);
	CHECK(math::Round(+2.90f, -1.0f) == +3.00f);
	CHECK(math::Round(+2.51f, -1.0f) == +3.00f);
	CHECK(math::Round(+2.50f, -1.0f) == +3.00f);
	CHECK(math::Round(+2.49f, -1.0f) == +2.00f);
	CHECK(math::Round(+2.10f, -1.0f) == +2.00f);
	CHECK(math::Round(+2.00f, -1.0f) == +2.00f);

	CHECK(math::Round(+3.00f, -0.5f) == +3.00f);
	CHECK(math::Round(+2.90f, -0.5f) == +3.00f);
	CHECK(math::Round(+2.51f, -0.5f) == +2.50f);
	CHECK(math::Round(+2.50f, -0.5f) == +2.50f);
	CHECK(math::Round(+2.49f, -0.5f) == +2.50f);
	CHECK(math::Round(+2.10f, -0.5f) == +2.00f);
	CHECK(math::Round(+2.00f, -0.5f) == +2.00f);

	// negative, positive multiplier
	CHECK(math::Round(-3.00f, +1.0f) == -3.00f);
	CHECK(math::Round(-2.90f, +1.0f) == -3.00f);
	CHECK(math::Round(-2.51f, +1.0f) == -3.00f);
	CHECK(math::Round(-2.50f, +1.0f) == -3.00f);
	CHECK(math::Round(-2.49f, +1.0f) == -2.00f);
	CHECK(math::Round(-2.10f, +1.0f) == -2.00f);
	CHECK(math::Round(-2.00f, +1.0f) == -2.00f);

	CHECK(math::Round(-3.00f, +0.5f) == -3.00f);
	CHECK(math::Round(-2.90f, +0.5f) == -3.00f);
	CHECK(math::Round(-2.51f, +0.5f) == -2.50f);
	CHECK(math::Round(-2.50f, +0.5f) == -2.50f);
	CHECK(math::Round(-2.49f, +0.5f) == -2.50f);
	CHECK(math::Round(-2.10f, +0.5f) == -2.00f);
	CHECK(math::Round(-2.00f, +0.5f) == -2.00f);

	// negative, negative multiplier
	CHECK(math::Round(-3.00f, -1.0f) == -3.00f);
	CHECK(math::Round(-2.90f, -1.0f) == -3.00f);
	CHECK(math::Round(-2.51f, -1.0f) == -3.00f);
	CHECK(math::Round(-2.50f, -1.0f) == -3.00f);
	CHECK(math::Round(-2.49f, -1.0f) == -2.00f);
	CHECK(math::Round(-2.10f, -1.0f) == -2.00f);
	CHECK(math::Round(-2.00f, -1.0f) == -2.00f);

	CHECK(math::Round(-3.00f, -0.5f) == -3.00f);
	CHECK(math::Round(-2.90f, -0.5f) == -3.00f);
	CHECK(math::Round(-2.51f, -0.5f) == -2.50f);
	CHECK(math::Round(-2.50f, -0.5f) == -2.50f);
	CHECK(math::Round(-2.49f, -0.5f) == -2.50f);
	CHECK(math::Round(-2.10f, -0.5f) == -2.00f);
	CHECK(math::Round(-2.00f, -0.5f) == -2.00f);
}