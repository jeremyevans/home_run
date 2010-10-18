require File.expand_path('../../spec_helper', __FILE__)

describe "Date#upto" do
  
  it "should be able to step forward in time" do
    ds    = Date.civil(2008, 10, 11)
    de    = Date.civil(2008,  9, 29)
    count = 0
    de.upto(ds) do |d|
      d.should <= ds
      d.should >= de
      count += 1
    end
    count.should == 13
  end


  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.jd.upto(c.jd + 2) do |d|
      d.should be_kind_of(c)
    end.should be_kind_of(c)
  end
end
