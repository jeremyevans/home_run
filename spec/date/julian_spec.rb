require File.expand_path('../../spec_helper', __FILE__)

describe "Date.jd" do
  it "should be able to construct a Date object based on the Julian day" do
    Date.jd(2454482).should == Date.civil(2008, 1, 16)
  end

  it "should be able to determine the Julian day for a Date object" do
    Date.civil(2008, 1, 16).jd.should == 2454482
  end
  
  it "should have defaults" do
    Date.jd.should == Date.jd(0)
    Date.jd(2008).should == Date.jd(2008)
    Date.jd(2008, 1).should == Date.jd(2008)
  end
  
  it "should not accept more than 2 arguments" do
    proc{Date.jd(2008, 1, 1)}.should raise_error(ArgumentError)
  end

  it "should raise ArgumentError for invalid dates" do
    proc{Date.jd(Date::GREGORIAN)}.should raise_error
    proc{Date.jd(Date::JULIAN)}.should raise_error
  end

  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.jd.should be_kind_of(c)
  end

  ruby_version_is "" ... "1.9" do
    it ".new1 should be the same as jd" do
      Date.new1(2454156).should == Date.jd(2454156)
    end
  end
end

describe "Date.new!" do
  it "should be the same as jd" do
    Date.new!(2454156).should == Date.jd(2454156)
  end
  
  it "should accept extra arguments" do
    Date.new!.should == Date.jd(0)
    Date.new!(2008).should == Date.jd(2008)
    Date.new!(2008, 1).should == Date.jd(2008)
    Date.new!(2008, 1, 1).should == Date.jd(2008)
  end
  
  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.new!.should be_kind_of(c)
  end

  it "should not accept more than 3 arguments" do
    proc{Date.new!(2008, 1, 1, 1)}.should raise_error(ArgumentError)
  end

  ruby_version_is "" ... "1.9" do
    it "#new0 should be the same as new!" do
      Date.new0(2454156).should == Date.new!(2454156)
    end
  end
end

describe "Date#julian?" do
  it "should return false" do
    Date.civil(1007, 2, 27).julian?.should == false
    Date.civil(1907, 2, 27, Date.civil(2000, 1, 1).jd).julian?.should == false
  end
  
  ruby_version_is "" ... "1.9" do
    it "os? should be same as julian?" do
      Date.civil(1007, 2, 27).os?.should == false
    end
  end
end

describe "Date.julian_leap?" do

  it "should be able to determine whether a year is a leap year in the Julian calendar" do
    Date.julian_leap?(1900).should == true
    Date.julian_leap?(1999).should == false
    Date.julian_leap?(2000).should == true
    Date.julian_leap?(2002).should == false
    Date.julian_leap?(2004).should == true
    Date.julian_leap?(1901).should == false
    Date.julian_leap?(1899).should == false
    Date.julian_leap?(1904).should == true
    Date.julian_leap?(1896).should == true
    Date.julian_leap?(1999).should == false
    Date.julian_leap?(2001).should == false
  end

end

describe "Date.valid_jd?" do
  ruby_version_is "" ... "1.9" do
    it "should be able to determine if a day number is a valid Julian day number, true for all numbers" do
      # This might need to check the type of the jd parameter. Date.valid_jd?(:number) is of course
      # bogus but returns itself with the current implementation
      Date.valid_jd?(-100).should == -100
      Date.valid_jd?(0).should    ==    0
      Date.valid_jd?(100).should  ==  100
      Date.valid_jd?(2454156).should == 2454156
      Date.valid_jd?(2454156, 1).should == 2454156
    end
    
    it "#exist1? should be the same as valid_jd?" do
      Date.exist1?(2454156, 1).should == Date.valid_jd?(2454156, 1)
    end
  end

  ruby_version_is "1.9" do
    it "should be able to determine if a day number is a valid Julian day number, true for all numbers" do
      # This might need to check the type of the jd parameter. Date.valid_jd?(:number) is of course
      # bogus but returns itself with the current implementation
      Date.valid_jd?(-100).should == true
      Date.valid_jd?(0).should    == true
      Date.valid_jd?(100).should  == true
      Date.valid_jd?(2454156).should == true
      Date.valid_jd?(2454156, 1).should == true
    end
  end
end

ruby_version_is "" ... "1.9" do
  describe "Date" do
    it ".julian? should return whether the first argument is less than the second" do
      Date.julian?(1, 1).should == false
      Date.julian?(1, 2).should == true
    end

    it ".os? should be the same as julian?" do
      Date.os?(1, 2).should == Date.julian?(1, 2)
    end
  end
end
