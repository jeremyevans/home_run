require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime conversions" do
  it "#new_offset should be a separate datetime with a modified offset" do
    DateTime.new(2008, 1, 1).new_offset(0.5).to_s.should == DateTime.new(2008, 1, 1, 12, 0, 0, 0.5).to_s
  end

  it "#new_offset should work with strings" do
    DateTime.new(2008, 1, 1).new_offset('+12:00').to_s.should == DateTime.new(2008, 1, 1, 12, 0, 0, 0.5).to_s
  end

  it "#new_offset result should be equal to the receiver" do
    DateTime.new(2008, 1, 1).new_offset(0.5).should == DateTime.new(2008, 1, 1)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.jd.new_offset.should be_kind_of(c)
  end

  ruby_version_is "" ... "1.9" do
    it "#newof should be a separate datetime with a modified offset" do
      DateTime.new(2008, 1, 1).newof(0.5).to_s.should == DateTime.new(2008, 1, 1, 12, 0, 0, 0.5).to_s
    end
  end
end

describe "DateTime marshalling" do
  it "should marshal and unmarshal correctly" do
    Marshal.load(Marshal.dump(DateTime.jd)).should == DateTime.civil
    d = DateTime.now
    Marshal.load(Marshal.dump(d)).should == d
    Marshal.load(Marshal.dump(DateTime.civil(2010, 2, 4, 1, 2, 4, 4/24.0))).should == DateTime.civil(2010, 2, 4, 1, 2, 4, 4/24.0)
  end
end

ruby_version_is "1.9" do
  describe "DateTime#to_datetime" do
    it " should return the receiver" do
      DateTime.new(2009, 1, 2, 3, 4, 5, 0.5).to_datetime.should == DateTime.new(2009, 1, 2, 3, 4, 5, 0.5)
    end
  end

  describe "DateTime#to_date" do
    it " should return a Date with the same date as receiver" do
      DateTime.new(2009, 1, 2, 12).to_date.should == Date.new(2009, 1, 2)
    end
  end

  describe "DateTime#to_time" do
    it " should return a Time in local time with the same year, month, day as the receiver" do
      DateTime.new(2009, 1, 2, 3, 4, 5).to_time.should == Time.utc(2009, 1, 2, 3, 4, 5).getlocal
    end
  end

  describe "Time#to_datetime" do
    it " should return a Date with the same year, month, and day as the receiver" do
      Time.utc(2009, 1, 2, 3, 4, 5).to_datetime.should == DateTime.new(2009, 1, 2, 3, 4, 5)
      Time.utc(2009, 1, 2, 3, 4, 5).getlocal.to_datetime.should == DateTime.new(2009, 1, 2, 3, 4, 5)
    end
  end
end

