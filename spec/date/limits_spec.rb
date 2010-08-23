require File.expand_path('../../spec_helper', __FILE__)

describe "Date limits: " do
  before do
    @d1 = Date.jd(Date::JULIAN - 1)
    @d2 = Date.jd(Date::GREGORIAN + 1)
  end

  it "#+ and #- should raise RangeError for dates that are too large or small" do
    proc{Date.jd(@d1.jd) + 1}.should raise_error(RangeError)
    proc{Date.jd(@d2.jd) - 1}.should raise_error(RangeError)
  end

  it "#<< and #>> should raise RangeError for dates that are too large or small" do
    proc{Date.jd(@d1.jd) >> 1}.should raise_error(RangeError)
    proc{Date.jd(@d2.jd) << 1}.should raise_error(RangeError)
  end

  ruby_version_is "1.9" do
    it "#next_year and #prev_year should raise RangeError for dates that are too large or small" do
      proc{Date.jd(@d1.jd).next_year}.should raise_error(RangeError)
      proc{Date.jd(@d2.jd).prev_year}.should raise_error(RangeError)
    end
  end

  it ".jd should raise RangeError for dates that are too large or small" do
    proc{Date.jd(@d1.jd + 1)}.should raise_error(RangeError)
    proc{Date.jd(@d2.jd - 1)}.should raise_error(RangeError)
  end

  it ".jd should not raise for dates that are not too large or small" do
    proc{Date.jd(@d1.jd)}.should_not raise_error
    proc{Date.jd(@d2.jd)}.should_not raise_error
  end

  it ".new! should raise RangeError for dates that are too large or small" do
    proc{Date.new!(@d1.jd + 1)}.should raise_error(RangeError)
    proc{Date.new!(@d2.jd - 1)}.should raise_error(RangeError)
  end

  it ".new! should not raise for dates that are not too large or small" do
    proc{Date.new!(@d1.jd)}.should_not raise_error
    proc{Date.new!(@d2.jd)}.should_not raise_error
  end

  it ".civil should raise RangeError for dates that are too large or small" do
    proc{Date.civil(@d1.year, @d1.month, @d1.day + 1)}.should raise_error(RangeError)
    proc{Date.civil(@d2.year, @d2.month, @d2.day - 1)}.should raise_error(RangeError)
  end

  it ".civil should not raise for dates that are not too large or small" do
    proc{Date.civil(@d1.year, @d1.month, @d1.day)}.should_not raise_error
    proc{Date.civil(@d2.year, @d2.month, @d2.day)}.should_not raise_error
  end

  it ".civil should correctly convert dates within limits to JD" do
    Date.civil(@d1.year, @d1.month, @d1.day).jd.should == @d1.jd
    Date.civil(@d2.year, @d2.month, @d2.day).jd.should == @d2.jd
  end

  it ".commercial should raise RangeError for dates that are too large or small" do
    proc{Date.commercial(@d1.cwyear, @d1.cwday == 6 ? @d1.cweek + 1 : @d1.cweek, @d1.cwday == 6 ? 1 : @d1.cwday + 1)}.should raise_error(RangeError)
    proc{Date.commercial(@d2.cwyear, @d2.cwday == 1 ? @d2.cweek - 1 : @d2.cweek, @d2.cwday == 1 ? 7 : @d2.cwday - 1)}.should raise_error(RangeError)
  end

  it ".commercial should not raise for dates that are not too large or small" do
    proc{Date.commercial(@d1.cwyear, @d1.cweek, @d1.cwday)}.should_not raise_error
    proc{Date.commercial(@d2.cwyear, @d2.cweek, @d2.cwday)}.should_not raise_error
  end

  it ".commercial should correctly convert dates within limits to JD" do
    Date.commercial(@d1.cwyear, @d1.cweek, @d1.cwday).jd.should == @d1.jd
    Date.commercial(@d2.cwyear, @d2.cweek, @d2.cwday).jd.should == @d2.jd
  end

  it ".ordinal should raise RangeError for dates that are too large or small" do
    proc{Date.ordinal(@d1.year, @d1.yday + 1)}.should raise_error(RangeError)
    proc{Date.ordinal(@d2.year, @d2.yday - 1)}.should raise_error(RangeError)
  end

  it ".ordinal should not raise for dates that are not too large or small" do
    proc{Date.ordinal(@d1.year, @d1.yday)}.should_not raise_error
    proc{Date.ordinal(@d2.year, @d2.yday)}.should_not raise_error
  end

  it ".ordinal should correctly convert dates within limits to JD" do
    Date.ordinal(@d1.year, @d1.yday).jd.should == @d1.jd
    Date.ordinal(@d2.year, @d2.yday).jd.should == @d2.jd
  end

  it ".parse should raise RangeError for civil dates that are too large or small" do
    proc{Date.parse("#{@d1.year}-#{@d1.month}-#{@d1.day+1}")}.should raise_error(RangeError)
    proc{Date.parse("#{@d2.year}-#{@d2.month}-#{@d2.day-1}")}.should raise_error(RangeError)
  end

  it ".parse should not raise for civil dates that are not too large or small" do
    proc{Date.parse("#{@d1.year}-#{@d1.month}-#{@d1.day}")}.should_not raise_error
    proc{Date.parse("#{@d2.year}-#{@d2.month}-#{@d2.day}")}.should_not raise_error
  end

  it ".parse should correctly convert civil dates within limits to JD" do
    Date.parse("#{@d1.year}-#{@d1.month}-#{@d1.day}").jd.should == @d1.jd
    Date.parse("#{@d2.year}-#{@d2.month}-#{@d2.day}").jd.should == @d2.jd
  end

  it ".strptime should raise RangeError for civil dates that are too large or small" do
    proc{Date.strptime("#{@d1.year}-#{@d1.month}-#{@d1.day+1}", "%Y-%m-%d")}.should raise_error(RangeError)
    proc{Date.strptime("#{@d2.year}-#{@d2.month}-#{@d2.day-1}", "%Y-%m-%d")}.should raise_error(RangeError)
  end

  it ".strptime should not raise for civil dates that are not too large or small" do
    proc{Date.strptime("#{@d1.year}-#{@d1.month}-#{@d1.day}", "%Y-%m-%d")}.should_not raise_error
    proc{Date.strptime("#{@d2.year}-#{@d2.month}-#{@d2.day}", "%Y-%m-%d")}.should_not raise_error
  end

  it ".strptime should correctly convert civil dates within limits to JD" do
    Date.strptime("#{@d1.year}-#{@d1.month}-#{@d1.day}", "%Y-%m-%d").jd.should == @d1.jd
    Date.strptime("#{@d2.year}-#{@d2.month}-#{@d2.day}", "%Y-%m-%d").jd.should == @d2.jd
  end

end
