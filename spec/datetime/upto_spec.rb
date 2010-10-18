require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#upto" do
  
  it "should be able to step forward in time" do
    ds    = DateTime.civil(2008, 10, 11)
    de    = DateTime.civil(2008,  9, 29)
    count = 0
    de.upto(ds) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end
    count.should == 13
  end

  it "should respect fractional days" do
    ds    = DateTime.civil(2008, 10, 11, 0)
    de    = DateTime.civil(2008,  9, 29, 12)
    count = 0
    de.upto(ds) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end
    count.should == 12

    ds    = DateTime.civil(2008, 10, 11, 12)
    de    = DateTime.civil(2008,  9, 29, 0)
    count = 0
    de.upto(ds) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end
    count.should == 13
  end

  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.jd.upto(c.jd + 2) do |d|
      d.should be_kind_of(c)
    end.should be_kind_of(c)
  end
end
