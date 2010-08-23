require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime limits: " do
  before do
    @d1 = DateTime.jd(DateTime::JULIAN - 1)
    @d2 = DateTime.jd(DateTime::GREGORIAN + 1)
  end

  it "#+ and #- should raise RangeError for dates that are too large or small" do
    proc{DateTime.jd(@d1.jd) + 1}.should raise_error(RangeError)
    proc{DateTime.jd(@d2.jd) - 1}.should raise_error(RangeError)
  end

  it "#<< and #>> should raise RangeError for dates that are too large or small" do
    proc{DateTime.jd(@d1.jd) >> 1}.should raise_error(RangeError)
    proc{DateTime.jd(@d2.jd) << 1}.should raise_error(RangeError)
  end

  ruby_version_is "1.9" do
    it "#next_year and #prev_year should raise RangeError for dates that are too large or small" do
      proc{DateTime.jd(@d1.jd).next_year}.should raise_error(RangeError)
      proc{DateTime.jd(@d2.jd).prev_year}.should raise_error(RangeError)
    end
  end

  it ".jd should raise RangeError for dates that are too large or small" do
    proc{DateTime.jd(@d1.jd + 1)}.should raise_error(RangeError)
    proc{DateTime.jd(@d2.jd - 1)}.should raise_error(RangeError)
  end

  it ".jd should not raise for dates that are not too large or small" do
    proc{DateTime.jd(@d1.jd)}.should_not raise_error
    proc{DateTime.jd(@d2.jd)}.should_not raise_error
  end

  it ".new! should raise RangeError for dates that are too large or small" do
    proc{DateTime.new!(@d1.jd + 1)}.should raise_error(RangeError)
    # Need to use 1.5 instead of 1 because new! adds a half day
    proc{DateTime.new!(@d2.jd - 1.5)}.should raise_error(RangeError)
  end

  it ".new! should not raise for dates that are not too large or small" do
    proc{DateTime.new!(@d1.jd)}.should_not raise_error
    proc{DateTime.new!(@d2.jd)}.should_not raise_error
  end

  it ".civil should raise RangeError for dates that are too large or small" do
    proc{DateTime.civil(@d1.year, @d1.month, @d1.day + 1)}.should raise_error(RangeError)
    proc{DateTime.civil(@d2.year, @d2.month, @d2.day - 1)}.should raise_error(RangeError)
  end

  it ".civil should not raise for dates that are not too large or small" do
    proc{DateTime.civil(@d1.year, @d1.month, @d1.day)}.should_not raise_error
    proc{DateTime.civil(@d2.year, @d2.month, @d2.day)}.should_not raise_error
  end

  it ".civil should correctly convert dates within limits to JD" do
    DateTime.civil(@d1.year, @d1.month, @d1.day).jd.should == @d1.jd
    DateTime.civil(@d2.year, @d2.month, @d2.day).jd.should == @d2.jd
  end

  it ".commercial should raise RangeError for dates that are too large or small" do
    proc{DateTime.commercial(@d1.cwyear, @d1.cwday == 6 ? @d1.cweek + 1 : @d1.cweek, @d1.cwday == 6 ? 1 : @d1.cwday + 1)}.should raise_error(RangeError)
    proc{DateTime.commercial(@d2.cwyear, @d2.cwday == 1 ? @d2.cweek - 1 : @d2.cweek, @d2.cwday == 1 ? 7 : @d2.cwday - 1)}.should raise_error(RangeError)
  end

  it ".commercial should not raise for dates that are not too large or small" do
    proc{DateTime.commercial(@d1.cwyear, @d1.cweek, @d1.cwday)}.should_not raise_error
    proc{DateTime.commercial(@d2.cwyear, @d2.cweek, @d2.cwday)}.should_not raise_error
  end

  it ".commercial should correctly convert dates within limits to JD" do
    DateTime.commercial(@d1.cwyear, @d1.cweek, @d1.cwday).jd.should == @d1.jd
    DateTime.commercial(@d2.cwyear, @d2.cweek, @d2.cwday).jd.should == @d2.jd
  end

  it ".ordinal should raise RangeError for dates that are too large or small" do
    proc{DateTime.ordinal(@d1.year, @d1.yday + 1)}.should raise_error(RangeError)
    proc{DateTime.ordinal(@d2.year, @d2.yday - 1)}.should raise_error(RangeError)
  end

  it ".ordinal should not raise for dates that are not too large or small" do
    proc{DateTime.ordinal(@d1.year, @d1.yday)}.should_not raise_error
    proc{DateTime.ordinal(@d2.year, @d2.yday)}.should_not raise_error
  end

  it ".ordinal should correctly convert dates within limits to JD" do
    DateTime.ordinal(@d1.year, @d1.yday).jd.should == @d1.jd
    DateTime.ordinal(@d2.year, @d2.yday).jd.should == @d2.jd
  end

  it ".parse should raise RangeError for civil dates that are too large or small" do
    proc{DateTime.parse("#{@d1.year}-#{@d1.month}-#{@d1.day+1}")}.should raise_error(RangeError)
    proc{DateTime.parse("#{@d2.year}-#{@d2.month}-#{@d2.day-1}")}.should raise_error(RangeError)
  end

  it ".parse should not raise for civil dates that are not too large or small" do
    proc{DateTime.parse("#{@d1.year}-#{@d1.month}-#{@d1.day}")}.should_not raise_error
    proc{DateTime.parse("#{@d2.year}-#{@d2.month}-#{@d2.day}")}.should_not raise_error
  end

  it ".parse should correctly convert civil dates within limits to JD" do
    DateTime.parse("#{@d1.year}-#{@d1.month}-#{@d1.day}").jd.should == @d1.jd
    DateTime.parse("#{@d2.year}-#{@d2.month}-#{@d2.day}").jd.should == @d2.jd
  end

  it ".strptime should raise RangeError for civil dates that are too large or small" do
    proc{DateTime.strptime("#{@d1.year}-#{@d1.month}-#{@d1.day+1}", "%Y-%m-%d")}.should raise_error(RangeError)
    proc{DateTime.strptime("#{@d2.year}-#{@d2.month}-#{@d2.day-1}", "%Y-%m-%d")}.should raise_error(RangeError)
  end

  it ".strptime should not raise for civil dates that are not too large or small" do
    proc{DateTime.strptime("#{@d1.year}-#{@d1.month}-#{@d1.day}", "%Y-%m-%d")}.should_not raise_error
    proc{DateTime.strptime("#{@d2.year}-#{@d2.month}-#{@d2.day}", "%Y-%m-%d")}.should_not raise_error
  end

  it ".strptime should correctly convert civil dates within limits to JD" do
    DateTime.strptime("#{@d1.year}-#{@d1.month}-#{@d1.day}", "%Y-%m-%d").jd.should == @d1.jd
    DateTime.strptime("#{@d2.year}-#{@d2.month}-#{@d2.day}", "%Y-%m-%d").jd.should == @d2.jd
  end

end

describe "DateTime offset limits: " do
  it "methods should raise ArgumentError for offets that are too large or small" do
    max = (14 + 1.0/60)/24
    min = -(14 + 1.0/60)/24
    proc{DateTime.civil(2009, 1, 1, 0, 0, 0, max)}.should raise_error(ArgumentError)
    proc{DateTime.civil(2009, 1, 1, 0, 0, 0, min)}.should raise_error(ArgumentError)
    proc{DateTime.commercial(2009, 1, 1, 0, 0, 0, max)}.should raise_error(ArgumentError)
    proc{DateTime.commercial(2009, 1, 1, 0, 0, 0, min)}.should raise_error(ArgumentError)
    proc{DateTime.ordinal(2009, 1, 0, 0, 0, max)}.should raise_error(ArgumentError)
    proc{DateTime.ordinal(2009, 1, 0, 0, 0, min)}.should raise_error(ArgumentError)
    proc{DateTime.jd(2009, 0, 0, 0, max)}.should raise_error(ArgumentError)
    proc{DateTime.jd(2009, 0, 0, 0, min)}.should raise_error(ArgumentError)
    proc{DateTime.new!(2009, max)}.should raise_error(ArgumentError)
    proc{DateTime.new!(2009, min)}.should raise_error(ArgumentError)
    proc{DateTime.parse("00:00:00+14:01", "%H:%M:%S%z")}.should raise_error(ArgumentError)
    proc{DateTime.parse("00:00:00-14:01", "%H:%M:%S%z")}.should raise_error(ArgumentError)
    proc{DateTime.strptime("00:00:00+14:01", "%H:%M:%S%z")}.should raise_error(ArgumentError)
    proc{DateTime.strptime("00:00:00-14:01", "%H:%M:%S%z")}.should raise_error(ArgumentError)

    d = DateTime.jd(2009)
    proc{d.new_offset(max)}.should raise_error(ArgumentError)
    proc{d.new_offset(min)}.should raise_error(ArgumentError)
  end

  it "methods should not raise for offsets that are not too large or small" do
    max = 14.0/24
    min = -14.0/24
    proc{DateTime.civil(2009, 1, 1, 0, 0, 0, max)}.should_not raise_error
    proc{DateTime.civil(2009, 1, 1, 0, 0, 0, min)}.should_not raise_error
    proc{DateTime.commercial(2009, 1, 1, 0, 0, 0, max)}.should_not raise_error
    proc{DateTime.commercial(2009, 1, 1, 0, 0, 0, min)}.should_not raise_error
    proc{DateTime.ordinal(2009, 1, 0, 0, 0, max)}.should_not raise_error
    proc{DateTime.ordinal(2009, 1, 0, 0, 0, min)}.should_not raise_error
    proc{DateTime.jd(2009, 0, 0, 0, max)}.should_not raise_error
    proc{DateTime.jd(2009, 0, 0, 0, min)}.should_not raise_error
    proc{DateTime.new!(2009, max)}.should_not raise_error
    proc{DateTime.new!(2009, min)}.should_not raise_error
    proc{DateTime.parse("00:00:00+14:00", "%H:%M:%S%z")}.should_not raise_error
    proc{DateTime.parse("00:00:00-14:00", "%H:%M:%S%z")}.should_not raise_error
    proc{DateTime.strptime("00:00:00+14:00", "%H:%M:%S%z")}.should_not raise_error
    proc{DateTime.strptime("00:00:00-14:00", "%H:%M:%S%z")}.should_not raise_error

    d = DateTime.jd(2009)
    proc{d.new_offset(max)}.should_not raise_error
    proc{d.new_offset(min)}.should_not raise_error
  end
end
